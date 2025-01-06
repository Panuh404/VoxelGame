#pragma once
#include <memory>
#include <string>

class Util
{
	Util() = default;

public:
	class HashVec2
	{
	public:
		size_t operator()(const glm::ivec2& coord) const noexcept
		{
			size_t hash = coord.x;
			hash ^= coord.y + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
	};

	class HashVec3
	{
	public:
		size_t operator()(const glm::ivec3& coord) const noexcept
		{
			size_t hash = coord.x;
			hash ^= coord.y + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= coord.z + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
	};

	class CompareIVec3
	{
	public:
		int operator()(const glm::ivec3& lhs, const glm::ivec3&  rhs) const
		{
			if (lhs.x < rhs.x) return true;
			if (lhs.x == rhs.x)
			{
				if (lhs.y < rhs.y) return true;
				if (lhs.y == rhs.y) return lhs.z < rhs.z;
				return false;
			}
			return false;
		}
	};

	static std::shared_ptr<std::string> ReadBinaryFile(const std::string& path);
	static int32_t PositiveMod(int32_t num, int32_t divisor);
};
