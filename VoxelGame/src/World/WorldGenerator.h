#pragma once
#include "Chunk.h"
#include "FastNoiseLite/Cpp/FastNoiseLite.h"

class WorldGenerator
{
	int32_t seed;
	FastNoiseLite noise;

public:
	WorldGenerator(int32_t seed);
	int32_t GetSeed() const { return seed; }

	void PopulateChunk(const std::shared_ptr<Chunk>& chunk);
};
