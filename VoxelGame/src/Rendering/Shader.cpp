#include "GamePCH.h"
#include "Shader.h"

#include "glad/glad.h"
#include "Scene/AssetManager/AssetManager.h"
#include "Utils/Log.h"

/////////////////////////////
/// SHADERS
/////////////////////////////
Shader::Shader(const std::string& source, uint32_t type)
{
	assert(type != 0 && "Couldn't identify the shader type");

	id = glCreateShader(type);
	const char *ShaderSourceStr = source.c_str();
	glShaderSource(id, 1, &ShaderSourceStr, nullptr);
	glCompileShader(id);

	// Verify compile status
	int32_t success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[2048];
		glGetShaderInfoLog(id, sizeof(infoLog) / sizeof(infoLog[0]), nullptr, infoLog);
		LOG_ERROR("Shader Compilation Failed: {0}", infoLog);
		id = 0;
	}
}

Shader::Shader(const std::string& name)
	: Shader(*AssetManager::instance().loadText(name), name.ends_with(".vert") ? GL_VERTEX_SHADER : (name.ends_with(".frag") ? GL_FRAGMENT_SHADER : 0)) {}

Shader::~Shader()
{
	if (isValid()) glDeleteShader(id);
}

/////////////////////////////
/// SHADER PROGRAM
/////////////////////////////
ShaderProgram::ShaderProgram(const std::shared_ptr<const Shader>& VertexShader, const std::shared_ptr<const Shader>& FragmentShader)
{
	assert(VertexShader->isValid());
	assert(FragmentShader->isValid());

	program = glCreateProgram();

	glAttachShader(program, VertexShader->getId());
	glAttachShader(program, FragmentShader->getId());
	glLinkProgram(program);

	// Verify linking status
	int32_t success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[2048];
		glGetProgramInfoLog(program, sizeof(infoLog) / sizeof(infoLog[0]), nullptr, infoLog);
		std::cerr << "Shader Program Linking Failed: {0}" << infoLog << std::endl;
		program = 0;
	}
}

ShaderProgram::ShaderProgram(const std::string& name)
	: ShaderProgram(AssetManager::instance().loadShader(name + ".vert"),
					AssetManager::instance().loadShader(name + ".frag")) {}

ShaderProgram::~ShaderProgram()
{
	if (isValid()) glDeleteProgram(program);
}

void ShaderProgram::Bind() const
{
	glUseProgram(program);
}

uint32_t ShaderProgram::GetUniformLocation(const std::string& location) const
{
	GLint uniformLocation = glGetUniformLocation(program, location.c_str());
	if (uniformLocation == -1) {
		LOG_ERROR("OpenGL Error: Uniform {0} not found!", location);
	}
	return uniformLocation;
}
void ShaderProgram::SetInt(const std::string& location, int32_t value) const
{
	glProgramUniform1i(program, GetUniformLocation(location), value);
}
void ShaderProgram::SetUInt(const std::string& location, int32_t value) const
{
	glProgramUniform1ui(program, GetUniformLocation(location), value);
}
void ShaderProgram::SetFloat(const std::string& location, float value) const
{
	glProgramUniform1f(program, GetUniformLocation(location), value);
}
void ShaderProgram::SetVec2(const std::string& location, const glm::vec2& value) const
{
	glProgramUniform2fv(program, GetUniformLocation(location), 1, &value.x);
}
void ShaderProgram::SetVec3(const std::string& location, const glm::vec3& value) const
{
	glProgramUniform3fv(program, GetUniformLocation(location), 1, &value.x);
}
void ShaderProgram::SetVec4(const std::string& location, const glm::vec4& value) const
{
	glProgramUniform4fv(program, GetUniformLocation(location), 1, &value.x);
}
void ShaderProgram::SetMat4(const std::string& location, const glm::mat4& value) const
{
	glProgramUniformMatrix4fv(program, GetUniformLocation(location), 1, GL_FALSE, &value[0][0]);
}
void ShaderProgram::SetTexture(const std::string& location, const std::shared_ptr<const Texture>& texture, int32_t slot) const
{
	texture->BindToSlot(slot);
	glProgramUniform1i(program, GetUniformLocation(location), slot);
}
