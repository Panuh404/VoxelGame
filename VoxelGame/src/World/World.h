#pragma once
#include "Chunk.h"
#include "WorldGenerator.h"
#include "Behavior/WorldBehavior.h"
#include "Math/Frustum.h"
#include "Rendering/Shader.h"
#include "Geometry/BlockData.h"
#include "Utils/Util.h"

class Framebuffer;

class World
{
	std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, Util::HashVec2> chunks;
	std::vector<std::shared_ptr<WorldBehavior>> behaviors;
	using ChunkIndexVector = std::vector<std::pair<glm::vec2, float>>;
	std::shared_ptr<const Texture> textureAtlas;
	std::shared_ptr<const ShaderProgram> opaqueShader;
	std::shared_ptr<const ShaderProgram> transparentShader;
	std::shared_ptr<const ShaderProgram> blendShader;
	bool useAmbientOcclusion = true;

	std::shared_ptr<Persistence> persistence;
	WorldGenerator generator;

	const uint32_t MaxRebuildsAllowedPerFrame = 10;

	int32_t viewDistance = 10;
	float textureAnimation = 0;
	static constexpr float TextureAnimationSpeed = 2;

	std::shared_ptr<Chunk> GenerateOrLoadChunk(glm::ivec2 position);
	void UnloadChunk(const std::shared_ptr<Chunk>& chunk);
	void SortChunkIndices(glm::vec3 playerPos, const std::shared_ptr<ChunkIndexVector>& chunkIndices);
	void RebuildChunks(const std::shared_ptr<ChunkIndexVector>& chunkIndices, const Frustum& frustum);

public:
	explicit World(const std::shared_ptr<Persistence>& persistence, std::vector<std::shared_ptr<WorldBehavior>> behaviors, int32_t seed);

	// Handle Chunks
	std::shared_ptr<Chunk> GetChunk(glm::ivec2 position);
	void AddChunk(glm::ivec2 position, const std::shared_ptr<Chunk>& chunk);
	static glm::ivec2 GetChunkIndex(glm::ivec3 position);
	bool IsChunkLoaded(glm::ivec2 position) const;

	int32_t GetViewDistance() const { return viewDistance; }
	void SetViewDistance(int32_t distance) { viewDistance = distance; }

	bool GetAmbientOcclusion() const { return useAmbientOcclusion; }
	void SetAmbientOcclusion(bool enabled) { useAmbientOcclusion = enabled; }

	// Handle Blocks
	const BlockData* GetBlockAt(glm::ivec3 position);
	const BlockData* GetBlockAtIfLoaded(glm::ivec3 position) const;
	bool PlaceBlock(BlockData block, glm::ivec3 position);
	static bool IsValidBlockPosition(glm::ivec3 position);

	// Rendering
	void Update(const glm::vec3& playerPosition, float deltaTime);
	void RenderTransparent(glm::mat4 transform, glm::vec3 playerPos, const Frustum& frustum, float zNear, float zFar, const std::shared_ptr<Framebuffer>& opaqueRender);
	void RenderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum& frustum);
	void SetTextureAtlas(const std::shared_ptr<const Texture>& texture);

};