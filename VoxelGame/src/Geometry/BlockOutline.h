#pragma once
#include "CubeMesh.h"
#include "Rendering/Shader.h"

class BlockOutline
{
	std::shared_ptr<const ShaderProgram> outlinedShader;
	std::shared_ptr<const CubeMesh> blockMesh;

public:
	explicit BlockOutline(std::shared_ptr<const CubeMesh> blockMesh);

	void Render(const glm::mat4& transform) const;
};
