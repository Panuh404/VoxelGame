#pragma once
#include "Shader.h"

class ProceduralShader
{
protected:
	virtual std::string EmitVertexShaderSource() const = 0;
	virtual std::string EmitFragmentShaderSource() const = 0;

public:
	explicit operator std::shared_ptr<const ShaderProgram>() const { return GetShader(); }
	std::shared_ptr<const ShaderProgram> GetShader() const;

	virtual ~ProceduralShader() = default;
};
