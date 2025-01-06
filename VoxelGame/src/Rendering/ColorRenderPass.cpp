#include "GamePCH.h"
#include "ColorRenderPass.h"

#include "GameSettings.h"
#include "Scene/AssetManager/AssetManager.h"

std::shared_ptr<VertexArray> FullscreenQuad::GetVertexArray()
{
	static std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>(std::vector<glm::vec3>
		{
			{ -1, -1, 0 }, 
			{ -1,  1, 0 },
			{  1, -1, 0 },
			{  1,  1, 0 }
		},
		std::vector<VertexAttribute>{VertexAttribute{ 3, VertexAttribute::Float, 0 }},
		std::vector<uint8_t>{0, 2, 1, 1, 2, 3});
	return vertexArray;
}

ColorRenderPass::ColorRenderPass(const std::shared_ptr<const ShaderProgram>& shader) : shader(shader)
{
	assert(shader != nullptr);
}

void ColorRenderPass::SetTexture(const std::string& attachmentName, const std::shared_ptr<Texture>& texture, int32_t slot)
{
	shader->SetTexture(attachmentName, texture, slot);
}

void ColorRenderPass::Render()
{
	shader->Bind();
	glDisable(GL_DEPTH_TEST);
	FullscreenQuad::GetVertexArray()->RenderIndexed();
	glEnable(GL_DEPTH_TEST);
}

void ColorRenderPass::RenderTextureWithEffect(const std::shared_ptr<Texture>& texture, const std::shared_ptr<const ShaderProgram>& effect)
{
	ColorRenderPass renderPass(effect);
	renderPass.SetTexture("colorTexture", texture, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	renderPass.Render();
}

void ColorRenderPass::RenderTexture(const std::shared_ptr<Texture>& texture)
{
	static std::shared_ptr<const ShaderProgram> colorIdentity = AssetManager::instance().loadShaderProgram(Settings::ShaderIdentity);
	RenderTextureWithEffect(texture, colorIdentity);
}
