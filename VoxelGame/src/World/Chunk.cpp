#include "GamePCH.h"
#include "Chunk.h"

#include "World.h"
#include "Geometry/BlockMesh.h"
#include "Geometry/BlockVertex.h"
#include "Utils/Util.h"

Chunk::Chunk(const glm::ivec2& worldPosition)
	: worldPosition(worldPosition), aabb(glm::vec3(0), glm::vec3(0))
{
	Init();
}

void Chunk::Init()
{
	solidVertexCount = 0;
	semiTransparentVertexCount = 0;
	mesh = nullptr;
	renderState = RenderState::Initial;

	glm::vec3 position = glm::vec3(worldPosition.x, 0, worldPosition.y);
	glm::vec3 maxOffset = glm::vec3(HorizontalSize, VerticalSize, HorizontalSize);
	aabb = AxisAlignedBoundingBox{ position, position + maxOffset };
}

void Chunk::RenderOpaque(const glm::mat4& transform, const Frustum& frustum)
{
    if (!mesh || !IsVisible(frustum)) return;

    shader->Bind();
    shader->SetMat4("MVP", transform * glm::translate(glm::vec3(worldPosition.x, 0, worldPosition.y)));

    if (solidVertexCount != 0)  mesh->RenderVertexSubStream(solidVertexCount, 0);    
}

void Chunk::RenderSemiTransparent(const glm::mat4& transform, const Frustum& frustum)
{
    if (!mesh || !IsVisible(frustum)) return;

    shader->Bind();
    shader->SetMat4("MVP", transform * glm::translate(glm::vec3(worldPosition.x, 0, worldPosition.y)));

    glDisable(GL_CULL_FACE);
    if (semiTransparentVertexCount != 0) mesh->RenderVertexSubStream(semiTransparentVertexCount, solidVertexCount);
    glEnable(GL_CULL_FACE);
}

const BlockData* Chunk::GetBlockAtOptimized(const glm::ivec3& pos, const World& world) const
{
    const glm::ivec2& worldPos = worldPosition;
    if (pos.y >= 0 && pos.y < Chunk::VerticalSize) 
    {
        if (pos.x >= 0 && pos.x < Chunk::HorizontalSize && pos.z >= 0 && pos.z < Chunk::HorizontalSize) 
        {
            return &blockData[pos.x][pos.y][pos.z];
        }
        else 
        {
            return world.GetBlockAtIfLoaded(glm::ivec3(pos.x + worldPos.x, pos.y, pos.z + worldPos.y));
        }
    }
    return nullptr;
}

bool hasNonAirAt(const glm::ivec3& pos, const Chunk& chunk, const World& world)
{
    const BlockData* block = chunk.GetBlockAtOptimized(pos, world);
    return block != nullptr && block->blockClass != BlockData::BlockClass::Air;
}

uint8_t calculateOcclusionLevel(const glm::ivec3& blockPos, const glm::ivec3& vertOffset, const Chunk& chunk, const World& world)
{
    glm::ivec3 direction = glm::sign(glm::vec3(vertOffset) - glm::vec3(.5));

    uint8_t side1 = hasNonAirAt(blockPos + direction * glm::ivec3(1, 1, 0), chunk, world) ? 1 : 0;
    uint8_t side2 = hasNonAirAt(blockPos + direction * glm::ivec3(0, 1, 1), chunk, world) ? 1 : 0;
    if (side1 && side2)  return 0;

    uint8_t corner = hasNonAirAt(blockPos + direction * glm::ivec3(1, 1, 1), chunk, world) ? 1 : 0;
    return 3 - (side1 + side2 + corner);
}

void Chunk::RebuildMesh(const World& world)
{
    static std::shared_ptr<std::vector<BlockVertex>> solidVertices = std::make_shared<std::vector<BlockVertex>>(MaxVertexCount);
    static std::shared_ptr<std::vector<BlockVertex>> semiTransparentVertices = std::make_shared<std::vector<BlockVertex>>(MaxVertexCount);

    solidVertexCount = 0;
    semiTransparentVertexCount = 0;

    const std::array<glm::ivec3, 6> offsetsToCheck = 
    {
    	{
    		{1, 0, 0},
    		{-1, 0, 0},
    		{0, 1, 0},
    		{0, -1, 0},
    		{0, 0, 1},
    		{0, 0, -1},
    	}
    };

    {
        for (int32_t x = HorizontalSize - 1; x >= 0; --x)
        {
            for (int32_t y = VerticalSize - 1; y >= 0; --y) 
            {
                for (int32_t z = HorizontalSize - 1; z >= 0; --z) 
                {
                    glm::ivec3 blockPos = { x, y, z };
                    const auto& [type, blockClass] = blockData[x][y][z];
                    if (blockClass == BlockData::BlockClass::Air) continue;

                    for (const glm::ivec3& offset : offsetsToCheck) 
                    {
                        const BlockData* block = GetBlockAtOptimized(blockPos + offset, world);
                        if (block == nullptr) continue;

                        bool isSameClass = block->blockClass == blockClass;
                        bool isTransparentNextToOpaque = block->blockClass == BlockData::BlockClass::Solid && blockClass == BlockData::BlockClass::Transparent;
                        if (isSameClass || isTransparentNextToOpaque)  continue;

                        for (const auto& vertex : BlockMesh::GetVerticesFromDirection(offset)) 
                        {
                            BlockVertex vert = vertex;
                            vert.Offset(x, y, z);
                            vert.SetType(offset, type);

                            uint8_t occlusionLevel = 3;
                            if (UseAmbientOcclusion) 
                            {
                                if (offset.y == -1) occlusionLevel = 0;
                                else occlusionLevel = calculateOcclusionLevel(blockPos, vert.GetPosition() - blockPos, *this, world);
                            }
                            vert.SetOcclusionLevel(occlusionLevel);

                            if (blockClass == BlockData::BlockClass::SemiTransparent || blockClass == BlockData::BlockClass::Transparent) 
                            {
                                semiTransparentVertices->at(semiTransparentVertexCount) = vert;
                                semiTransparentVertexCount++;
                            }
                            else 
                            {
                                solidVertices->at(solidVertexCount) = vert;
                                solidVertexCount++;
                            }
                        }
                    }
                }
            }
        }
    }
    {
        int32_t vertexCount = solidVertexCount + semiTransparentVertexCount;

        if (!mesh)
        {
            mesh = std::make_shared<VertexArray>();
            mesh->AddVertexAttributes(BlockVertex::VertexAttributes(), sizeof(BlockVertex));
        }

        std::shared_ptr<VertexBuffer> buffer = mesh->GetVertexBuffer();
        if (buffer->GetSize() < vertexCount) 
        {
            int32_t dataSize = glm::min(vertexCount + 1000, MaxVertexCount);
            buffer->BufferDynamicData(*solidVertices, dataSize, 0);
        }
        else 
        {
            buffer->BufferDynamicSubData(*solidVertices, solidVertexCount, 0, 0);
        }

        buffer->BufferDynamicSubData(*semiTransparentVertices, semiTransparentVertexCount, 0, solidVertexCount);
        renderState = RenderState::Ready;
    }
}

glm::ivec3 Chunk::ToChunkCoordinates(const glm::ivec3& globalPosition)
{
    return
	{
		Util::PositiveMod(globalPosition.x, HorizontalSize), globalPosition.y,
		Util::PositiveMod(globalPosition.z, HorizontalSize)
	};
}
