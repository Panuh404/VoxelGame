#include "GamePCH.h"
#include "BlockVertex.h"

BlockVertex::BlockVertex(const glm::ivec3& position, const glm::bvec2& uv)
{
	Offset(position.x, position.y, position.z);
	SetUV(uv.x, uv.y);
}

void BlockVertex::SetUV(bool x, bool y)
{
	uint8_t uv = x | (y << 1);
	assert((((data >> 19) & 0xff) + uv) <= 0xff && "UV Coordinates are out of bounds");

	data += uv << 19;
}

void BlockVertex::SetTexture(uint8_t x, uint8_t y)
{
	assert(x < 16 && "Coordinate is out of bounds");
	assert(y < 16 && "Coordinate is out of bounds");

	uint16_t uv = x | (y << 4);
	assert(uv <= 0xff && "Texture index is out of bounds");

	data += uv << 20;
}

void BlockVertex::Offset(uint32_t x, uint32_t y, uint32_t z)
{
	assert((((data >> 9) & 0x1fu) + x) <= 16 && "Coordinate is out of bounds");
	assert((((data >> 14) & 0x1fu) + z) <= 16 && "Coordinate is out of bounds");
	assert(((data & 0x1ffu) + y) <= 256 && "Coordinate is out of bounds");

	data += y;
	data += x << 9;
	data += z << 14;
}

void BlockVertex::SetAnimated()
{
	data |= 0b1 << 28;
}

void BlockVertex::SetType(const glm::ivec3& offset, BlockData::BlockType type)
{
    switch (type) {
    case BlockData::BlockType::Bedrock:     SetTexture(1, 1); break;
    case BlockData::BlockType::Planks:      SetTexture(4, 0); break;
    case BlockData::BlockType::Water:       SetAnimated(); SetTexture(13, 12); break;
    case BlockData::BlockType::Lava:        SetAnimated(); SetTexture(13, 14); break;
    case BlockData::BlockType::Iron:        SetTexture(6, 1); break;
    case BlockData::BlockType::Diamond:     SetTexture(8, 1); break;
    case BlockData::BlockType::Gold:        SetTexture(7, 1); break;
    case BlockData::BlockType::Obsidian:    SetTexture(5, 2); break;
    case BlockData::BlockType::Sponge:      SetTexture(0, 3); break;
    case BlockData::BlockType::Grass:
    	if (offset.y == 1) SetTexture(0, 0);
        else if (offset.y == -1) SetTexture(2, 0);
        else SetTexture(3, 0);
        break;
    case BlockData::BlockType::Dirt:        SetTexture(2, 0); break;
    case BlockData::BlockType::Sand:        SetTexture(2, 1); break;
    case BlockData::BlockType::Stone:       SetTexture(1, 0); break;
    case BlockData::BlockType::Cobblestone: SetTexture(0, 1); break;
    case BlockData::BlockType::Glass:       SetTexture(1, 3); break;
    case BlockData::BlockType::Oak_Wood:
        if (offset.y == 1 || offset.y == -1) SetTexture(5, 1);
        else SetTexture(4, 1);
        break;
    case BlockData::BlockType::Oak_Leaves:  SetTexture(4, 3); break;
    case BlockData::BlockType::Air:         assert(false); break;
    default: assert(false);
	}
}

glm::ivec3 BlockVertex::GetPosition() const
{
	return { (data >> 9) & 0x1fu, data & 0x1ffu, (data >> 14) & 0x1fu };
}

void BlockVertex::SetOcclusionLevel(uint8_t occlusionLevel)
{
	assert(occlusionLevel < 4 && "The occlusion level is out of bounds");
	data |= occlusionLevel << 29;
}
