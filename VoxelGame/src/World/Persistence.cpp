#include "GamePCH.h"
#include "Persistence.h"

#define SERIALIZE_DATA

Persistence::Persistence(std::string newPath) : path(std::move(newPath))
{
    #ifdef SERIALIZE_DATA
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) 
    {
        LOG_ERROR("Failed to open the file: {0}", path);
        return;
    }

    file.seekg(0, std::ios::end);
    auto length = size_t(file.tellg());
    if (length == -1) 
    {
        LOG_ERROR("Failed to read the file: {0}", path);
        return;
    }

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(&camera), sizeof(camera));
    size_t chunkCount = (length - sizeof(Camera)) / (sizeof(glm::ivec2) + sizeof(Chunk::blockData));

    for (size_t i = 0; i < chunkCount; i++) 
    {
        glm::ivec2 worldPosition;
        file.read(reinterpret_cast<char*>(&worldPosition[0]), sizeof(glm::ivec2));

        std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(worldPosition);
        file.read(reinterpret_cast<char*>(&chunk->blockData[0]), sizeof(Chunk::blockData));

        chunks[worldPosition] = chunk;
    }
    #endif
}

Persistence::~Persistence()
{
    #ifdef SERIALIZE_DATA
    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (!file) 
    {
        LOG_ERROR("Failed to open the file: {0}", path);
        return;
    }

    file.write(reinterpret_cast<char*>(&camera), sizeof(camera));

    for (auto& [key, chunk] : chunks) 
    {
        glm::ivec2 worldPosition = key;
        file.write(reinterpret_cast<char*>(&worldPosition[0]), sizeof(glm::ivec2));
        file.write(reinterpret_cast<char*>(&chunk->blockData[0]), sizeof(Chunk::blockData));
    }
    #endif
}

void Persistence::CommitChunk(const std::shared_ptr<Chunk>& chunk)
{
    #ifdef SERIALIZE_DATA
    chunks[chunk->GetPosition()] = chunk;
    #endif
}

std::shared_ptr<Chunk> Persistence::GetChunk(glm::ivec2 position) const
{
    if (!chunks.contains(position)) 
    {
        return nullptr;
    }
    return chunks.at(position);
}

void Persistence::CommitCamera(const Camera& newCamera)
{
    #ifdef SERIALIZE_DATA
    camera = newCamera;
    #endif
}

const Camera& Persistence::GetCamera() const
{
    return camera;
}