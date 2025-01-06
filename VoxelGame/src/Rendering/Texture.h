#pragma once
#include "GamePCH.h"
#include "Image.h"

class Texture {
	uint32_t id = 0;
	uint32_t target;
	int32_t internalFormat;
	int32_t format;
	int32_t type;
	bool generateMipMap;

public:
	Texture(uint32_t target, int32_t internalFormat, int32_t format, int32_t type, bool generateMipMap = true, int32_t maxLod = 1000);
	~Texture();

	[[nodiscard]] bool IsValid() const { return id != 0; }
	[[nodiscard]] uint32_t GetId() const { return id; }

	// Binding
	void Bind() const;
	void BindToSlot(uint32_t slot) const;
	void Unbind() const;

	// Allocation
	void AllocateTexture(int32_t width, int32_t height);
	void Buffer2DRGBAData(const Image& image);
	void Buffer2DArrayRGBAData(std::span<const Image> images);
	void BufferCubeMapRGBAData(std::span<std::shared_ptr<const Image>, 6> images);

	// Loading
	static std::shared_ptr<const Texture> LoadTexture2D(const std::string& name);
	static std::shared_ptr<const Texture> LoadTexture2DArray(const std::string& name);
	static std::shared_ptr<const Texture> LoadCubeMapTexture(const std::string& name);
};
