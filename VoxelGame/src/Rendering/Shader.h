#pragma once
#include <memory>
#include <string>

#include "Texture.h"
#include "glm/glm.hpp"


class Shader
{
	uint32_t id;

public:
	Shader(const std::string& source, uint32_t type);
	explicit Shader(const std::string &name);
	~Shader();

	bool isValid() const { return id != 0; }
	uint32_t getId() const { return id; }
};


class ShaderProgram
{
	uint32_t program = 0;
	uint32_t GetUniformLocation(const std::string &location) const;

public:
	ShaderProgram(const std::shared_ptr<const Shader>& VertexShader, const std::shared_ptr<const Shader>& FragmentShader);
	explicit ShaderProgram(const std::string &name);
	~ShaderProgram();

	bool isValid() const { return program; }
	void Bind() const;

	void SetInt(const std::string &location, int32_t value) const;
	void SetUInt(const std::string &location, int32_t value) const;
	void SetFloat(const std::string &location, float value) const;
	void SetVec2(const std::string &location, const glm::vec2 &value) const;
	void SetVec3(const std::string &location, const glm::vec3 &value) const;
	void SetVec4(const std::string &location, const glm::vec4 &value) const;
	void SetMat4(const std::string &location, const glm::mat4 &value) const;
	void SetTexture(const std::string &location, const std::shared_ptr<const Texture> &texture, int32_t slot) const;
};