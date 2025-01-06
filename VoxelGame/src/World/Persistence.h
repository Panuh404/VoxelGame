#pragma once
#include "Chunk.h"
#include "Scene/Camera.h"
#include "Utils/Util.h"

class Persistence
{
	std::string path;
	Camera camera;
	std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, Util::HashVec2> chunks;

public:
	explicit Persistence(std::string path);
	~Persistence();

	void CommitChunk(const std::shared_ptr<Chunk>& chunk);
	void CommitCamera(const Camera& newCamera);

	std::shared_ptr<Chunk> GetChunk(glm::ivec2 position) const;
	const Camera& GetCamera() const;
};
