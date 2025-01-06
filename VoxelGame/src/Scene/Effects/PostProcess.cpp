#include "GamePCH.h"
#include "PostProcess.h"

#include "Application/Window.h"
#include "Rendering/ColorRenderPass.h"

PostProcess::PostProcess(const std::shared_ptr<const ShaderProgram>& shader, bool enabled) : shader(shader), enabled(enabled) {}

void PostProcess::Render()
{
	if (!enabled) return;

	Window& window = Window::instance();
	int32_t width = window.GetWindowWidth();
	int32_t height = window.GetWindowHeight();
	if (framebuffer == nullptr || framebuffer->GetWidth() != width || framebuffer->GetHeight() != height) {
		framebuffer = std::make_shared<Framebuffer>(width, height, false, 1);
	}

	std::shared_ptr<FramebufferStack> framebufferStack = window.GetFramebufferStack();
	std::shared_ptr<Framebuffer> colorSource = framebufferStack->Peek();
	framebufferStack->Push(framebuffer);

	Update();
	ColorRenderPass::RenderTextureWithEffect(colorSource->GetColorAttachment(0), shader);

	std::shared_ptr<Framebuffer> resultFbo = framebufferStack->Pop();
	ColorRenderPass::RenderTexture(resultFbo->GetColorAttachment(0));
}


