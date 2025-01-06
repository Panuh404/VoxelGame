#pragma once
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/VertexArray.h"

class Skybox
{
	glm::mat4 transform{ 1 };
	VertexArray vertexArray{
     std::vector<glm::vec3>{
        {1, 1, 1},
        {1, 1, -1},
        {1, -1, 1},
        {1, -1, -1},
        {-1, 1, 1},
        {-1, 1, -1},
        {-1, -1, 1},
        {-1, -1, -1},
     },
     std::vector<VertexAttribute>{{3, VertexAttribute::Float, 0}},
     std::vector<uint8_t>{0, 1, 5, 4, 0, 5, 2, 1, 0, 1, 2, 3, 7, 4, 5, 4, 7, 6,
                          1, 3, 7, 1, 7, 5, 4, 6, 0, 2, 0, 6, 3, 2, 6, 7, 3, 6},
    };

    std::shared_ptr<const Texture> cubeMap;
    std::shared_ptr<const ShaderProgram> shader;

    float rotation = 0;
    float rotationSpeed = 0.01;

public:
    Skybox();

    float GetRotationSpeed() const { return rotationSpeed; }
    void SetRotationSpeed(float speed) { rotationSpeed = speed; }

    void Update(const glm::mat4& projection, const glm::mat4& cameraView, float deltaTime);
    void Render();
};
