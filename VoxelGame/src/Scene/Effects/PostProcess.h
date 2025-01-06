#pragma once
#include "Rendering/Framebuffer.h"
#include "Rendering/Shader.h"

class PostProcess
{
protected:
	bool enabled;
	std::shared_ptr<const ShaderProgram> shader;
	std::shared_ptr<Framebuffer> framebuffer;

public:
	explicit PostProcess(const std::shared_ptr<const ShaderProgram>& shader, bool enabled = false);
	virtual ~PostProcess() = default;

	std::shared_ptr<const ShaderProgram> GetShader() { return shader; }

	virtual void Update() = 0;
	virtual void RenderGUI() = 0;
	void Render();

};
