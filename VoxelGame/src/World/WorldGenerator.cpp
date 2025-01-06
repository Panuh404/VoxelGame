#include "GamePCH.h"
#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(int32_t seed)
	: seed(seed), noise(seed)
{
	noise.SetFractalOctaves(5);
	noise.SetFractalLacunarity(1.75);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
}

void WorldGenerator::PopulateChunk(const std::shared_ptr<Chunk>& chunk)
{
	Chunk& m_Chunk = *chunk;

	glm::ivec2 worldPosition = m_Chunk.GetPosition();
	glm::vec2 position = worldPosition;

	for (int32_t x = 0; x < Chunk::HorizontalSize; x++)
	{
		for (int32_t z = 0; z < Chunk::HorizontalSize; z++)
		{
			float noiseX = (position.x + static_cast<float>(x));
			float noiseY = (position.y + static_cast<float>(z));
			float noiseValue = noise.GetNoise(noiseX, noiseY) / 2.0f + 0.5f;
			int32_t height = 45 + static_cast<int32_t>(noiseValue * 45);

			for (int32_t y = 0; y < height; y++)
			{
				int32_t dy = height - y;
				BlockData::BlockType blockToPlace = BlockData::BlockType::Stone;

				if (dy == 1)
				{
					if (y <= 64 && y >= 63) blockToPlace = BlockData::BlockType::Sand;
					else if (y <= 63)		blockToPlace = BlockData::BlockType::Stone;
					else					blockToPlace = BlockData::BlockType::Grass;
				}
				else if (dy < 5)
				{
					if (y < 64)				blockToPlace = BlockData::BlockType::Stone;
					else					blockToPlace = BlockData::BlockType::Dirt;
				}
				m_Chunk.PlaceBlock(blockToPlace, x, y, z);
			}

			for (int32_t y = 64; y >= height; y--)
			{
				m_Chunk.PlaceBlock(BlockData::BlockType::Water, x, y, z);
			}

			chunk->PlaceBlock(BlockData::BlockType::Bedrock, x, 0, z);
		}
	}
}
