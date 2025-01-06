#pragma once
#include "BlockData.h"
#include "Rendering/VertexArray.h"

class BlockVertex
{
	uint32_t data = 0;

	void SetUV(bool x, bool y);
	void SetTexture(uint8_t x, uint8_t y);
public:
	BlockVertex() = default;
	BlockVertex(const glm::ivec3& position, const glm::bvec2& uv);

	void Offset(uint32_t x, uint32_t y, uint32_t z);
	void SetAnimated();
	void SetType(const glm::ivec3& offset, BlockData::BlockType type);
	glm::ivec3 GetPosition() const;

	static std::vector<VertexAttribute> VertexAttributes() { return { VertexAttribute(1, VertexAttribute::UInt, 0) }; }
	void SetOcclusionLevel(uint8_t occlusionLevel);
};

static_assert(sizeof(BlockVertex) == sizeof(uint32_t), "The BlockVertex struct must not have padding");

