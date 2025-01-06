#include "GamePCH.h"
#include "World.h"

#include <ranges>

#include "GameSettings.h"
#include "Persistence.h"
#include "Application/Window.h"
#include "Rendering/ColorRenderPass.h"
#include "Rendering/Framebuffer.h"
#include "Scene/AssetManager/AssetManager.h"

World::World(const std::shared_ptr<Persistence>& persistence, std::vector<std::shared_ptr<WorldBehavior>> behaviors, int32_t seed)
	: behaviors(std::move(behaviors)), persistence(persistence), generator(seed)
{
	opaqueShader = AssetManager::instance().loadShaderProgram(Settings::ShaderOpaqueSource);
	transparentShader = AssetManager::instance().loadShaderProgram(Settings::ShaderTransparentSource);
	blendShader = AssetManager::instance().loadShaderProgram(Settings::ShaderBlendSource);
	SetTextureAtlas(AssetManager::instance().loadTextureArray(Settings::TextureAtlasSource));
}

std::shared_ptr<Chunk> World::GenerateOrLoadChunk(glm::ivec2 position)
{
    std::shared_ptr<Chunk> chunk = persistence->GetChunk(position);
    if (chunk != nullptr) return chunk;
    
    chunk = std::make_shared<Chunk>(position);
    generator.PopulateChunk(chunk);
    persistence->CommitChunk(chunk);

    return chunk;
}

void World::UnloadChunk(const std::shared_ptr<Chunk>& chunk)
{
    const auto chunkPos = chunk->GetPosition();
    chunks.erase(chunkPos);

    for (int32_t x = 0; x < Chunk::HorizontalSize; ++x) 
    {
        for (int32_t y = 0; y < Chunk::VerticalSize; ++y) 
        {
            for (int32_t z = 0; z < Chunk::HorizontalSize; ++z) 
            {
                for (const auto& worldBehavior : behaviors) 
                {
                    glm::ivec3 blockPos = { x, y, z };
                    glm::ivec3 globalBlockPos = blockPos + glm::ivec3(chunkPos.x, 0, chunkPos.y);
                    worldBehavior->OnBlockRemoved(globalBlockPos, chunk->GetBlockAt(blockPos), *this, false);
                }
            }
        }
    }
}

void World::Update(const glm::vec3& playerPosition, float deltaTime)
{
    textureAnimation += deltaTime * TextureAnimationSpeed;
    glm::vec2 playerChunkPosition = GetChunkIndex(playerPosition);

    auto chunksCopy = chunks;
    float unloadDistance = static_cast<float>(viewDistance + 1) * 16 + 8.0f;

    for (const auto& [chunkPosition, chunk] : chunksCopy) 
    {
        if (glm::abs(glm::distance(glm::vec2(chunkPosition), playerChunkPosition)) > unloadDistance) 
        {
            UnloadChunk(chunk);
        }
    }

    float loadDistance = static_cast<float>(viewDistance) * 16 + 8.0f;
    for (int32_t i = -viewDistance; i <= viewDistance; i++) 
    {
        for (int32_t j = -viewDistance; j <= viewDistance; j++) 
        {
            glm::ivec2 position = glm::ivec2(i * 16, j * 16) + glm::ivec2(playerChunkPosition);
            if (IsChunkLoaded(position)) continue;

            float distance = glm::abs(glm::distance(glm::vec2(position), playerChunkPosition));
            if (distance <= loadDistance) 
            {
                AddChunk(position, GenerateOrLoadChunk(position));
            }
        }
    }

    for (auto& behavior : behaviors) 
    {
        behavior->Update(deltaTime);
    }
}

void World::SortChunkIndices(glm::vec3 playerPos, const std::shared_ptr<ChunkIndexVector>& chunkIndices)
{
    chunkIndices->clear();
    if (chunkIndices->capacity() < chunks.size()) 
    {
        chunkIndices->reserve(chunks.size());
    }

    glm::vec2 playerXZ = glm::vec2(playerPos.x, playerPos.z);
    for (const auto& [key, value] : chunks) 
    {
        chunkIndices->emplace_back(key, value->DistanceToPoint(playerXZ));
    }

    std::sort(chunkIndices->begin(), chunkIndices->end(),
        [](const auto& a, const auto& b) { return b.second < a.second; });
}

void World::RebuildChunks(const std::shared_ptr<ChunkIndexVector>& chunkIndices, const Frustum& frustum)
{
    uint32_t meshesRebuilt = 0;
    for (auto& index : std::ranges::reverse_view(*chunkIndices)) 
    {
        if (meshesRebuilt > MaxRebuildsAllowedPerFrame) break;
        
        const auto& chunk = chunks[index.first];
        if (chunk->NeedsMeshRebuild() && chunk->IsVisible(frustum)) 
        {
            chunk->RebuildMesh(*this);
            meshesRebuilt++;
        }
    }
}

void World::RenderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum& frustum)
{
    static auto sortedChunkIndices = std::make_shared<ChunkIndexVector>();
    SortChunkIndices(playerPos, sortedChunkIndices);
    RebuildChunks(sortedChunkIndices, frustum);

    const int32_t animationProgress = static_cast<int32_t>(textureAnimation) % 5;

    // Fluid Animations
    const static int32_t animationOffsets[] = { 0, 1, 2, 17, 18 };
    const int32_t animationOffset = animationOffsets[animationProgress];
    opaqueShader->SetUInt("textureAnimation", animationOffset);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (const auto& index : *sortedChunkIndices)
    {
        const auto& chunk = chunks[index.first];
        chunk->SetShader(opaqueShader);
        chunk->SetUseAmbientOcclusion(useAmbientOcclusion);
        chunk->RenderOpaque(transform, frustum);
        chunk->RenderSemiTransparent(transform, frustum);
    }

    for (const auto& behavior : behaviors) 
    {
        behavior->RenderOpaque(transform, playerPos, frustum);
    }

    glDisable(GL_BLEND);
}

void World::RenderTransparent(glm::mat4 transform, glm::vec3 playerPos, const Frustum& frustum, float zNear, float zFar, const std::shared_ptr<Framebuffer>& opaqueRender)
{
    auto width = opaqueRender->GetWidth();
    auto height = opaqueRender->GetHeight();
    static std::shared_ptr<Framebuffer> framebuffer = nullptr;

    if (framebuffer == nullptr || framebuffer->GetWidth() != width || framebuffer->GetHeight() != height)
    {
        framebuffer = std::make_shared<Framebuffer>(width, height, false, 2);
    }
    static auto sortedChunkIndices = std::make_shared<ChunkIndexVector>();
    SortChunkIndices(playerPos, sortedChunkIndices);
    RebuildChunks(sortedChunkIndices, frustum);

    const int32_t animationProgress = static_cast<int32_t>(textureAnimation) % 5;

    // Fluid Animations
    const static int32_t animationOffsets[] = { 0, 1, 2, 17, 18 };
    const int32_t animationOffset = animationOffsets[animationProgress];
    auto& window = Window::instance();
    window.GetFramebufferStack()->Push(framebuffer);

    glEnable(GL_BLEND);
    glBlendFunci(0, GL_ONE, GL_ONE);
    glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    framebuffer->ClearColorAttachment(1, glm::vec4(1));

    transparentShader->SetUInt("textureAnimation", animationOffset);
    transparentShader->SetFloat("zNear", zNear);
    transparentShader->SetFloat("zFar", zFar);
    transparentShader->SetTexture("opaqueDepth", opaqueRender->GetDepthAttachment(), 1);
    transparentShader->Bind();

    for (const auto& [key, chunk] : chunks) 
    {
        chunk->SetShader(transparentShader);
        chunk->SetUseAmbientOcclusion(useAmbientOcclusion);
        chunk->RenderSemiTransparent(transform, frustum);
    }
    Window::instance().GetFramebufferStack()->Pop();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ColorRenderPass renderPass(blendShader);
    renderPass.SetTexture("accumTexture", framebuffer->GetColorAttachment(0), 1);
    renderPass.SetTexture("revealageTexture", framebuffer->GetColorAttachment(1), 2);
    renderPass.SetTexture("opaqueTexture", opaqueRender->GetColorAttachment(0), 3);
    renderPass.Render();
    glDisable(GL_BLEND);
}

const BlockData* World::GetBlockAt(glm::ivec3 position)
{
    return GetChunk(GetChunkIndex(position))->GetBlockAt(Chunk::ToChunkCoordinates(position));
}

bool World::IsValidBlockPosition(glm::ivec3 position)
{
    return Chunk::IsValidPosition(position);
}


bool World::PlaceBlock(BlockData block, glm::ivec3 position)
{
    if (!Chunk::IsValidPosition(position)) 
    {
        return false;
    }

    glm::ivec3 positionInChunk = Chunk::ToChunkCoordinates(position);
    auto chunk = GetChunk(GetChunkIndex(position));
    auto oldBlock = chunk->GetBlockAt(positionInChunk);

    for (const auto& behavior : behaviors) 
    {
        behavior->OnBlockRemoved(position, oldBlock, *this, true);
    }

    GetChunk(GetChunkIndex(position))->PlaceBlock(block, positionInChunk);
    for (const auto& behavior : behaviors) 
    {
        behavior->OnNewBlock(position, &block, *this);
    }

    constexpr std::array<glm::ivec3, 6> blocksAround = 
    {
    	{
    		{0, 0, 1},
    		{1, 0, 0},
    		{0, 0, -1},
    		{-1, 0, 0},
    		{0, 1, 0},
    		{0, -1, 0}
    	}
    };

    for (const glm::ivec3& offset : blocksAround) 
    {
        glm::ivec3 neighbor = offset + positionInChunk;
        glm::ivec3 neighborWorldPosition = position + offset;
        if (!Chunk::IsInBounds(neighbor.x, neighbor.y, neighbor.z)) 
        {
            const auto& chunk = GetChunk(GetChunkIndex(neighborWorldPosition));
            chunk->SetDirty();
        }
        for (const auto& behavior : behaviors) 
        {
            behavior->OnBlockUpdate(neighborWorldPosition, GetBlockAt(neighborWorldPosition), *this);
        }
    }

    return true;
}

glm::ivec2 World::GetChunkIndex(glm::ivec3 position)
{
    return {
    		position.x - Util::PositiveMod(position.x, Chunk::HorizontalSize),
            position.z - Util::PositiveMod(position.z, Chunk::HorizontalSize) };
}

std::shared_ptr<Chunk> World::GetChunk(glm::ivec2 position)
{
    if (!IsChunkLoaded(position)) 
    {
        AddChunk(position, GenerateOrLoadChunk(position));
    }
    return chunks.at(position);
}

void World::AddChunk(glm::ivec2 position, const std::shared_ptr<Chunk>& chunk)
{
    chunks[position] = chunk;
    std::array<glm::ivec2, 4> chunksAround = 
    {
    	{
    		{0, 16},
    		{16, 0},
    		{0, -16},
    		{-16, 0}
    	}
    };

    for (const glm::ivec2& offset : chunksAround) 
    {
        glm::ivec2 neighborPosition = position + offset;
        if (!IsChunkLoaded(neighborPosition)) continue;

        chunks[neighborPosition]->SetDirty();
    }

    for (int32_t x = 0; x < Chunk::HorizontalSize; ++x) 
    {
        for (int32_t y = 0; y < Chunk::VerticalSize; ++y) 
        {
            for (int32_t z = 0; z < Chunk::HorizontalSize; ++z) 
            {
                for (const auto& worldBehavior : behaviors) 
                {
                    glm::ivec3 blockPos = { x, y, z };
                    worldBehavior->OnNewBlock(blockPos + glm::ivec3(position.x, 0, position.y), chunk->GetBlockAt(blockPos), *this);
                }
            }
        }
    }
}

void World::SetTextureAtlas(const std::shared_ptr<const Texture>& texture)
{
    textureAtlas = texture;
    opaqueShader->SetTexture("atlas", textureAtlas, 0);
    transparentShader->SetTexture("atlas", textureAtlas, 0);
}

const BlockData* World::GetBlockAtIfLoaded(glm::ivec3 position) const
{
    glm::ivec2 index = GetChunkIndex(position);
    if (!IsChunkLoaded(index)) return nullptr;

    return chunks.at(index)->GetBlockAt(Chunk::ToChunkCoordinates(position));
}

bool World::IsChunkLoaded(glm::ivec2 position) const
{
    return chunks.contains(position);
}