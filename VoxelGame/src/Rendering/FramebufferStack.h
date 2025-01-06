#pragma once
#include "Framebuffer.h"

class FramebufferStack
{
	std::vector<std::shared_ptr<Framebuffer>> stack;
	std::vector<std::shared_ptr<Texture>> intermediateTextures;
	bool keepIntermediateTextures = false;

public:
	void Push(const std::shared_ptr<Framebuffer>& framebuffer);
	std::shared_ptr<Framebuffer> Peek() const;
	std::shared_ptr<Framebuffer> Pop();

	void SetKeepIntermediateTextures(bool keepBuffers);
	void ClearIntermediateTextureReferences();
	std::vector<std::shared_ptr<Texture>> GetIntermediateTextures() const;

	bool Empty() const;
	size_t Size() const;
};
