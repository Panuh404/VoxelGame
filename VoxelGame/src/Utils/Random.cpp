#include "GamePCH.h"
#include "Random.h"

Random::Random() : randomizer(std::random_device()()) {}

float Random::GetFloat()
{
	return distribution(randomizer) / static_cast<float>(std::numeric_limits<std::mt19937::result_type>::max());
}

glm::vec2 Random::GetVec2()
{
	return glm::vec2(GetFloat(), GetFloat());
}

glm::vec3 Random::GetVec3()
{
	return glm::vec3(GetFloat(), GetFloat(), GetFloat());
}
