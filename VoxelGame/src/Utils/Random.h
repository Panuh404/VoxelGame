#pragma once

class Random
{
	std::mt19937 randomizer;
	std::uniform_int_distribution<std::mt19937::result_type> distribution;

public:
	Random();

	float GetFloat();
	glm::vec2 GetVec2();
	glm::vec3 GetVec3();
};

