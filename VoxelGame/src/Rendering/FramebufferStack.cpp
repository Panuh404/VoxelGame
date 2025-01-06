#include "GamePCH.h"
#include "FramebufferStack.h"

#include "glm/gtx/quaternion.hpp"

void FramebufferStack::Push(const std::shared_ptr<Framebuffer>& framebuffer)
{
	stack.push_back(framebuffer);
	framebuffer->Bind();

	if (keepIntermediateTextures) 
	{
		for (int i = 0; i < framebuffer->GetAttachmentCount(); ++i) 
		{
			auto texture = framebuffer->GetColorAttachment(i);
			intermediateTextures.push_back(texture);
		}
	}
}

std::shared_ptr<Framebuffer> FramebufferStack::Peek() const
{
	return Empty() ? nullptr : stack.back();
}

std::shared_ptr<Framebuffer> FramebufferStack::Pop()
{
	assert(!Empty());

	std::shared_ptr<Framebuffer> framebuffer = Peek();
	stack.pop_back();

	auto current = Peek();
	if (current != nullptr) 
	{
		current->Bind();
	}
	else 
	{
		framebuffer->Unbind();
	}

	return framebuffer;
}

void FramebufferStack::SetKeepIntermediateTextures(bool keepBuffers)
{
	keepIntermediateTextures = keepBuffers;
}

void FramebufferStack::ClearIntermediateTextureReferences()
{
	intermediateTextures.clear();
}

std::vector<std::shared_ptr<Texture>> FramebufferStack::GetIntermediateTextures() const
{
	return intermediateTextures;
}

bool FramebufferStack::Empty() const
{
	return stack.empty();
}

size_t FramebufferStack::Size() const
{
	return stack.size();
}
