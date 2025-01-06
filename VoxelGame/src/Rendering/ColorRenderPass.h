#pragma once
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace FullscreenQuad {
	std::shared_ptr<VertexArray> GetVertexArray();
}

class ColorRenderPass
{
	std::shared_ptr<const ShaderProgram> shader;

public:
	explicit ColorRenderPass(const std::shared_ptr<const ShaderProgram>& shader);

	void SetTexture(const std::string& attachmentName, const std::shared_ptr<Texture>& texture, int32_t slot);
	void Render();

	static void RenderTextureWithEffect(const std::shared_ptr<Texture>& texture, const std::shared_ptr<const ShaderProgram>& effect);
	static void RenderTexture(const std::shared_ptr<Texture>& texture);
};
