#pragma once
#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "glad/glad.h"

class Buffer
{
protected:
	Buffer(uint32_t type) : type(type) { glGenBuffers(1, &id); }

	int32_t size = 0;
	uint32_t id = 0;
	uint32_t type;

public:
	~Buffer() { if (IsValid()) glDeleteBuffers(1, &id); }

	void Bind() { glBindBuffer(type, id); }

	int32_t GetSize() const { return size; }
	uint32_t GetId() const { return id; };
	bool IsValid() const { return id != 0; };

	template<typename T>
	void BufferStaticData(const std::vector<T> &data, int32_t dataSize, int32_t dataOffset = 0)
	{
		assert(IsValid() && "Cannot write data to an invalid buffer");
		assert(dataOffset + dataSize <= data.size() && "Data is out of bounds");

		Bind();
		size = dataSize;
		glBufferData(type, sizeof(T) * size, &data[dataOffset], GL_STATIC_DRAW);
	}

	template<typename T>
	void BufferStaticData(const std::vector<T> &data)
	{
		BufferStaticData(data, data.size());
	}

	template<typename T>
	void BufferDynamicData(const std::vector<T> &data, int32_t dataSize, int32_t dataOffset = 0)
	{
		assert(IsValid() && "Cannot write data to an invalid buffer");
		assert(dataOffset + dataSize <= data.size() && "Data is out of bounds");

		Bind();
		size = dataSize;
		glBufferData(type, sizeof(T) * size, &data[dataOffset], GL_DYNAMIC_DRAW);
	}

	template<typename T>
	void BufferDynamicData(const std::vector<T> &data) {
		BufferDynamicData(data, data.size());
	}

	template<typename T>
	void BufferDynamicSubData(const std::vector<T> &data, int32_t dataSize, int32_t dataOffset = 0, int32_t bufferOffset = 0)
	{
		assert(IsValid() && "Cannot write data to an invalid buffer");
		assert(dataOffset + dataSize <= data.size() && "Data is out of bounds");
		assert(dataOffset + dataSize <= size && "Buffer is out of bounds");

		Bind();
		size = dataSize;
		glBufferSubData(type, bufferOffset * sizeof(T), sizeof(T) * size, &data[dataOffset]);
	}
};

class VertexBuffer : public Buffer
{
public:
	VertexBuffer() : Buffer(GL_ARRAY_BUFFER) {}
	static std::shared_ptr<VertexBuffer> Create() { return std::make_shared<VertexBuffer>(); }
};

class IndexBuffer : public Buffer
{
	uint32_t type = 0;

	template<typename T>
	int32_t GetSizeType() {
		switch (sizeof(T))
		{
		case 1: return GL_UNSIGNED_BYTE;
		case 2: return GL_UNSIGNED_SHORT;
		case 4: return GL_UNSIGNED_INT;
		}
		assert(false);
		return 0;
	}

public:
	template<typename T>
	explicit IndexBuffer(T _) : Buffer(GL_ELEMENT_ARRAY_BUFFER) {
		static_assert(
			std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value || std::is_same<T, uint32_t>::value,
			"The given type must be either unsigned char, unsigned short or unsigned int");
		type = GetSizeType<T>();
	}

	template<typename T>
	static std::shared_ptr<IndexBuffer> Create()
	{
		return std::make_shared<IndexBuffer>(T());
	}

	uint32_t GetType() const { return type; }
};