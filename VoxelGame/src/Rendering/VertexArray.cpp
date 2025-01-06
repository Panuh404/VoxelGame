#include "GamePCH.h"
#include "VertexArray.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &id);
	Bind();

	vertexBuffer = VertexBuffer::Create();
	vertexBuffer->Bind();

	Unbind();
}

VertexArray::~VertexArray()
{
	if (IsValid()) glDeleteVertexArrays(1, &id);
}

void VertexArray::Bind()
{
	glBindVertexArray(id);
	if (vertexBuffer) vertexBuffer->Bind();
	if (indexBuffer) indexBuffer->Bind();
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexAttributes(const std::vector<VertexAttribute>& vector, int32_t defaultVertexSize)
{
	Bind();
	for (size_t i = 0; i < vector.size(); i++) 
	{
		const auto &[componentCount, type, shouldBeNormalized, vertexSize, offset] = vector[i];
		const auto normalize = shouldBeNormalized ? GL_TRUE : GL_FALSE;
		const auto stride = vertexSize ? vertexSize : defaultVertexSize;

		switch (type)
		{
		case VertexAttribute::UShort:
		case VertexAttribute::Int:
		case VertexAttribute::UInt:
		case VertexAttribute::Byte:
			glVertexAttribIPointer(i, componentCount, type, stride, reinterpret_cast<void *>(offset));
			break;
		case VertexAttribute::Float:
			glVertexAttribPointer(i, componentCount, type, normalize, stride, reinterpret_cast<void *>(offset));
			break;
		}
		glEnableVertexAttribArray(i);
	}
	Unbind();
}

void VertexArray::RenderIndexed(int32_t type)
{
	assert(indexBuffer != nullptr);

	Bind();
	glDrawElements(type, indexBuffer->GetSize(), indexBuffer->GetType(), nullptr);
	Unbind();
}

void VertexArray::RenderVertexStream(int32_t type)
{
	RenderVertexSubStream(vertexBuffer->GetSize(), 0, type);
}

void VertexArray::RenderVertexSubStream(int32_t size, int32_t startOffset, int32_t type)
{
	if (!IsValid()) return;
	assert(indexBuffer == nullptr);

	Bind();
	glDrawArrays(type, startOffset, size);
	Unbind();
}