#pragma once
#include <cstdint>
#include <memory>

#include "Buffer.h"
#include "glad/glad.h"

struct VertexAttribute
{
	enum ComponentType
	{
		UShort = GL_UNSIGNED_SHORT,
		Int = GL_INT,
		UInt = GL_UNSIGNED_INT,
		Byte = GL_BYTE,
		Float = GL_FLOAT,
	};

	uint8_t componentCount;
	ComponentType type;
	bool shouldBeNormalized = false;
	int32_t vertexSize;
	uint32_t offset;

	VertexAttribute(uint8_t componentCount, ComponentType type, uint32_t offset)
		:	componentCount(componentCount),
			type(type),
			shouldBeNormalized(false),
			vertexSize(0),
			offset(offset) {}
};


class VertexArray
{
	uint32_t id = 0;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

public:
	explicit VertexArray();
	~VertexArray();

	template<typename VertexT>
	explicit VertexArray(const std::vector<VertexT> &vertices, bool dynamic = false) {
		glGenVertexArrays(1, &id);
		Bind();

		vertexBuffer = VertexBuffer::Create();
		if (dynamic) vertexBuffer->BufferDynamicData<VertexT>(vertices);
		else vertexBuffer->BufferStaticData<VertexT>(vertices);

		Unbind();
	}

	template<typename VertexT, typename IndexT>
	VertexArray(const std::vector<VertexT> &vertices, const std::vector<IndexT> &indices, bool dynamic = false)
		: VertexArray(vertices, dynamic)
	{
		Bind();
		indexBuffer = IndexBuffer::Create<IndexT>();

		if (dynamic) indexBuffer->BufferDynamicData<IndexT>(indices);
		else indexBuffer->BufferStaticData<IndexT>(indices);
		
		Unbind();
	}

	template<typename VertexT>
	explicit VertexArray(const std::vector<VertexT> &vertices,
		const std::vector<VertexAttribute> &vertexAttributes,
		bool dynamic = false)
		: VertexArray(vertices, dynamic)
	{
		AddVertexAttributes(vertexAttributes, sizeof(VertexT));
	}

	template<typename VertexT, typename IndexT>
	VertexArray(const std::vector<VertexT> &vertices, const std::vector<VertexAttribute> &vertexAttributes, const std::vector<IndexT> &indices, bool dynamic = false)
		: VertexArray(vertices, indices, dynamic)
	{
		AddVertexAttributes(vertexAttributes, sizeof(VertexT));
	}

	void Bind();
	void Unbind();
	void AddVertexAttributes(const std::vector<VertexAttribute> &vector, int32_t defaultVertexSize);

	void RenderIndexed(int32_t type = GL_TRIANGLES);
	void RenderVertexSubStream(int32_t size, int32_t startOffset, int32_t type = GL_TRIANGLES);
	void RenderVertexStream(int32_t type = GL_TRIANGLES);

	std::shared_ptr<VertexBuffer> GetVertexBuffer() { return vertexBuffer; }
	bool IsValid() const { return id != 0; }
};
