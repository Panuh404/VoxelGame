#include "GamePCH.h"
#include "Framebuffer.h"


Framebuffer::Framebuffer(int32_t width, int32_t height, bool createDepthAttachment, int32_t colorAttachmentCount) : width(width), height(height)
{
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	if (createDepthAttachment) 
	{
		depthAttachment = std::make_shared<Texture>(GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, false);
		depthAttachment->AllocateTexture(width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment->GetId(), 0);
	}

	for (uint32_t i = 0; i < colorAttachmentCount; ++i) 
	{
		std::shared_ptr<Texture> attachment = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA16, GL_RGBA, GL_SHORT, false, 0);
		attachment->AllocateTexture(width, height);

		uint32_t attachmentName = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentName, GL_TEXTURE_2D, attachment->GetId(), 0);

		attachmentNames.push_back(attachmentName);
		attachments.push_back(attachment);
	}
	CheckFramebufferStatus();
	Unbind();
}


Framebuffer::~Framebuffer()
{
	if (id) glDeleteFramebuffers(1, &id);
}

void Framebuffer::Bind(bool forDrawing)
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	CheckFramebufferStatus();
	if (forDrawing) glDrawBuffers(static_cast<int32_t>(attachmentNames.size()), attachmentNames.data());
	GLCheckError();
	
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLCheckError();
}

std::shared_ptr<Texture> Framebuffer::GetDepthAttachment()
{
	return depthAttachment;
}

std::shared_ptr<Texture> Framebuffer::GetColorAttachment(int32_t index)
{
	assert(index < attachments.size());
	return attachments[index];
}

void Framebuffer::ClearColorAttachment(int32_t index, const glm::vec4& value)
{
	Bind();
	assert(index < attachments.size());
	glClearBufferfv(GL_COLOR, index, &value.x);
	GLCheckError();
}
void Framebuffer::CheckFramebufferStatus()
{
	#ifdef DEBUG_FRAMEBUFFER
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		LogFramebufferDetails();
	}
	switch (status) {					
	case GL_FRAMEBUFFER_COMPLETE:						LOG_INFO("Framebuffer is complete"); break;
	case GL_FRAMEBUFFER_UNDEFINED:						LOG_ERROR("Framebuffer Error: GL_FRAMEBUFFER_UNDEFINED"); break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			LOG_ERROR("Framebuffer Error: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	LOG_ERROR("Framebuffer Error: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			LOG_ERROR("Framebuffer Error: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			LOG_ERROR("Framebuffer Error: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); break;
	case GL_FRAMEBUFFER_UNSUPPORTED:					LOG_ERROR("Framebuffer Error: GL_FRAMEBUFFER_UNSUPPORTED"); break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			LOG_ERROR("Framebuffer Error: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"); break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		LOG_ERROR("Framebuffer Error: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"); break;
	default:											LOG_ERROR("Framebuffer Error: Unknown error code {0}", status); break;
	}
	#endif
}

void Framebuffer::LogFramebufferDetails()
{
	GLint type, name;
	for (int i = 0; i < attachmentNames.size(); i++) {
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type);
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &name);
		LOG_INFO("Attachment {0}: Type={1}, Name={2}", i, type, name);
	}
	if (depthAttachment) {
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type);
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &name);
		LOG_INFO("Depth Attachment: Type={0}, Name={1}", type, name);
	}
}
