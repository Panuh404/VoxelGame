#include "GamePCH.h"
#include "ProceduralShader.h"

std::shared_ptr<const ShaderProgram> ProceduralShader::GetShader() const
{
	std::shared_ptr<const Shader> vertexShader = std::make_shared<const Shader>(EmitVertexShaderSource(), GL_VERTEX_SHADER);
	std::shared_ptr<const Shader> fragmentShader = std::make_shared<const Shader>(EmitFragmentShaderSource(), GL_FRAGMENT_SHADER);

	return std::make_shared<const ShaderProgram>(vertexShader, fragmentShader);
}
