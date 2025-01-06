#pragma once
#include "Rendering/VertexArray.h"

class CubeMesh
{
	std::shared_ptr<VertexArray> cubeVertexArray;

public:
	CubeMesh();
	void Render() const;
};
