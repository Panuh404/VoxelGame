#include "GamePCH.h"
#include "Util.h"
#include "Log.h"


std::shared_ptr<std::string> Util::ReadBinaryFile(const std::string& path)
{
	std::shared_ptr<std::string> content = std::make_shared<std::string>();
	std::ifstream file(path, std::ios::in | std::ios::binary);

	if (!file)
	{
		LOG_ERROR("Failed to open the file: {0}", path);
		return nullptr;
	}
	file.seekg(0, std::ios::end);
	auto length = file.tellg();

	if (length == -1)
	{
		LOG_ERROR("Failed to read the file: {0}", path);
		return nullptr;
	}
	content->resize(length);
	file.seekg(0, std::ios::beg);
	file.read(&content->at(0), length);

	return content;
}

int32_t Util::PositiveMod(int32_t num, int32_t divisor)
{
	int32_t mod = num % divisor;
	if (mod < 0) return mod + divisor;
	return mod;
}
