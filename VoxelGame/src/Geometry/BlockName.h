#pragma once
#include "BlockData.h"

struct BlockName
{
private:
	using Name = std::pair<BlockData::BlockType, const char*>;

public:
	static constexpr size_t BlockCount = 17;
	using NameArray = std::array<const char*, BlockCount>;
	static constexpr std::array<Name, BlockCount> BlockNames
	{
		{
			{ BlockData::BlockType::Grass, "Grass" },
			{ BlockData::BlockType::Dirt, "Dirt" },
			{ BlockData::BlockType::Stone, "Stone" },
			{ BlockData::BlockType::Cobblestone, "Cobblestone" },
			{ BlockData::BlockType::Sand, "Sand" },
			{ BlockData::BlockType::Glass, "Glass" },
			{ BlockData::BlockType::Oak_Wood, "Oak Wood" },
			{ BlockData::BlockType::Oak_Leaves, "Oak Leaves" },
			{ BlockData::BlockType::Bedrock, "Bedrock" },
			{ BlockData::BlockType::Planks, "Wooden Planks" },
			{ BlockData::BlockType::Water, "Water" },
			{ BlockData::BlockType::Lava, "Lava" },
			{ BlockData::BlockType::Iron, "Block of Iron" },
			{ BlockData::BlockType::Diamond, "Block of Diamond" },
			{ BlockData::BlockType::Gold, "Block of Gold" },
			{ BlockData::BlockType::Obsidian, "Obsidian" },
			{ BlockData::BlockType::Sponge, "Sponge" },
		}
	};

	static consteval NameArray GetBlockNames()
	{
		NameArray names{};
		for (int32_t i = 0; i < BlockCount; ++i)
		{
			names[i] = BlockNames[i].second;
		}

		return names;
	}

	static int32_t BlockTypeToIndex(BlockData::BlockType type);
	static const char* BlockTypeToName(BlockData::BlockType type);
};
