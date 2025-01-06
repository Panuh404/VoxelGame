#pragma once

struct BlockData
{
	enum class BlockClass
	{
		Air,
		Solid,
		SemiTransparent,
		Transparent
	};

	enum class BlockType
	{
		Bedrock,
		Planks,
		Grass,
		Dirt,
		Sand,
		Stone,
		Cobblestone,
		Glass,
		Oak_Wood,
		Oak_Leaves,
		Water,
		Lava,
		Iron,
		Diamond,
		Gold,
		Obsidian,
		Sponge,
		Air
	};

	BlockType type;
	BlockClass blockClass;

	glm::vec4 GetColor() const
	{
		switch (type)
		{
		case BlockType::Bedrock:	return glm::vec4(0.341, 0.341, 0.341, 1);
		case BlockType::Planks:		return glm::vec4(0.706, 0.565, 0.353, 1);
		case BlockType::Grass:		return glm::vec4(0.376, 0.627, 0.212, 1);
		case BlockType::Dirt:		return glm::vec4(0.588, 0.424, 0.29, 1);
		case BlockType::Sand:		return glm::vec4(0.82, 0.792, 0.576, 1);
		case BlockType::Stone:		return glm::vec4(0.498, 0.498, 0.498, 1);
		case BlockType::Cobblestone:return glm::vec4(0.427, 0.427, 0.427, 1);
		case BlockType::Glass:		return glm::vec4(0.996, 0.996, 0.996, 1);
		case BlockType::Oak_Wood:	return glm::vec4(0.416, 0.333, 0.204, 1);
		case BlockType::Oak_Leaves: return glm::vec4(0.114, 0.506, 0.114, 1);
		case BlockType::Water:		return glm::vec4(0.216, 0.325, 0.655, 1);
		case BlockType::Lava:		return glm::vec4(0.988, 0.631, 0., 1);
		case BlockType::Iron:		return glm::vec4(0.914, 0.914, 0.914, 1);
		case BlockType::Diamond:	return glm::vec4(0.412, 0.875, 0.855, 1);
		case BlockType::Gold:		return glm::vec4(0.996, 0.984, 0.365, 1);
		case BlockType::Obsidian:	return glm::vec4(0.035, 0.035, 0.055, 1);
		case BlockType::Sponge:		return glm::vec4(0.898, 0.898, 0.306, 1);
		default:					return glm::vec4(1, 1, 1, 1);
		}
	}

	static BlockClass TypeToClass(BlockType type)
	{
		if (type == BlockType::Air) return BlockClass::Air;
		if (type == BlockType::Water) return BlockClass::SemiTransparent;
		if (type == BlockType::Oak_Leaves || type == BlockType::Glass) return BlockClass::Transparent;
		return BlockClass::Solid;
	}

	BlockData(BlockType type = BlockType::Air) : type(type), blockClass(TypeToClass(type)) {}
};