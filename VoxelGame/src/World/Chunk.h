#pragma once

#include "GameSettings.h"
#include "Math/AxisAlignedBoundingBox.h"
#include "Math/Frustum.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Geometry/BlockData.h"

class Persistence;
class World;

class Chunk
{
	enum class RenderState
	{
		Initial,
		Ready,
		Dirty
	};
	int32_t solidVertexCount;
	int32_t semiTransparentVertexCount;
	std::shared_ptr<VertexArray> mesh;
	std::shared_ptr<const ShaderProgram> shader;

	bool UseAmbientOcclusion = true;

	RenderState renderState;
	glm::ivec2 worldPosition;

	void Init();

public:
	explicit Chunk(const glm::ivec2& worldPosition);

	static constexpr int32_t HorizontalSize = Settings::ChunkHorizontalSize;
	static constexpr int32_t VerticalSize = Settings::ChunkVerticalSize;

	static constexpr int32_t BlockCount = HorizontalSize * HorizontalSize * VerticalSize;
	static constexpr int32_t MaxVertexCount = BlockCount * 8;

	void RenderOpaque(const glm::mat4& transform, const Frustum& frustum);
	void RenderSemiTransparent(const glm::mat4& transform, const Frustum& frustum);
	void RebuildMesh(const World& world);


	bool NeedsMeshRebuild() const { return !mesh || renderState != RenderState::Ready; }
	void SetShader(const std::shared_ptr<const ShaderProgram>& newShader) { shader = newShader; }
	void SetDirty() { renderState = RenderState::Dirty; }
	void SetUseAmbientOcclusion(bool enabled)
	{
		if (enabled == UseAmbientOcclusion) return;
		SetDirty();
		UseAmbientOcclusion = enabled;
	}

	bool IsVisible(const Frustum& frustum) const
	{
		return frustum.IsBoxVisible(aabb.minPoint, aabb.maxPoint);
	};

	void PlaceBlock(BlockData block, const glm::ivec3& position)
	{
		PlaceBlock(block, position.x, position.y, position.z);
	}

	void PlaceBlock(BlockData block, int32_t x, int32_t y, int32_t z)
	{
		assert(IsInBounds(x, y, z));

		renderState = RenderState::Dirty;
		blockData[x][y][z] = block;
	}

	float DistanceToPoint(const glm::vec2& point) const
	{
		glm::vec2 referencePoint = 
		{
			glm::clamp(point.x, (float)worldPosition.x, (float)worldPosition.x + 16.0f),
			glm::clamp(point.y, (float)worldPosition.y, (float)worldPosition.y + 16.0f)
		};
		return glm::distance(referencePoint, point);
	}

	const BlockData* GetBlockAt(const glm::ivec3& position) const
	{
		return &blockData[position.x][position.y][position.z];
	}

	static bool IsInBounds(int32_t x, int32_t y, int32_t z)
	{
		return
			x >= 0 && x < HorizontalSize && 
			y >= 0 && y < VerticalSize && z >= 0 && 
			z < HorizontalSize;
	}

	const BlockData* GetBlockAtOptimized(const glm::ivec3& pos, const World& world) const;
	static bool IsValidPosition(glm::ivec3 position)
	{
		return position.y >= 0 && position.y < VerticalSize;
	}
	static glm::ivec3 ToChunkCoordinates(const glm::ivec3& globalPosition);

	glm::ivec2 GetPosition() { return worldPosition; }
private:
	BlockData blockData[HorizontalSize][VerticalSize][HorizontalSize];
	AxisAlignedBoundingBox aabb;

public:
	friend Persistence;
};
