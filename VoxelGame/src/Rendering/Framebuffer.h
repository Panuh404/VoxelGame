#pragma once
#include "Texture.h"

class Framebuffer
{
	uint32_t id = 0;

	std::vector<std::shared_ptr<Texture>> attachments;
	std::vector<uint32_t> attachmentNames;
	std::shared_ptr<Texture> depthAttachment = nullptr;

	int32_t width;
	int32_t height;

	void CheckFramebufferStatus();
	void LogFramebufferDetails();

public:
	Framebuffer(int32_t width, int32_t height, bool createDepthAttachment, int32_t colorAttachmentCount);
	~Framebuffer();

	int32_t GetWidth() const { return width; }
	int32_t GetHeight() const { return height; }
	size_t GetAttachmentCount() const { return attachments.size(); }
	bool isValid() const { return id != 0; }
	uint32_t getId() const { return id; }

	void Bind(bool forDrawing = true);
	void Unbind();

	std::shared_ptr<Texture> GetDepthAttachment();
	std::shared_ptr<Texture> GetColorAttachment(int32_t index);
	void ClearColorAttachment(int32_t index, const glm::vec4 &value);
};