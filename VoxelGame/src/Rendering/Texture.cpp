#include "GamePCH.h"
#include "Texture.h"

#include "GameSettings.h"
#include "Scene/AssetManager/AssetManager.h"

Texture::Texture(uint32_t target, int32_t internalFormat, int32_t format, int32_t type, bool generateMipMap, int32_t maxLod)
	: target(target), internalFormat(internalFormat), format(format), type(type), generateMipMap(generateMipMap)
{
	assert(target == GL_TEXTURE_2D || target == GL_TEXTURE_2D_ARRAY || target == GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &id);
	Bind();

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, generateMipMap ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (generateMipMap) 
	{
		glTexParameteri(target, GL_TEXTURE_MAX_LOD, maxLod);
	}
	Unbind();
}

Texture::~Texture()
{
	if (IsValid()) glDeleteTextures(1, &id);
}

void Texture::Bind() const
{
	glBindTexture(target, id);
	GLCheckError();
}

void Texture::BindToSlot(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	Bind();
}

void Texture::Unbind() const
{
	glBindTexture(target, 0);
}

void Texture::AllocateTexture(int32_t width, int32_t height)
{
	assert(target == GL_TEXTURE_2D);
	Bind();
	glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, nullptr);
	if (generateMipMap) 
	{
		glGenerateMipmap(target);
	}
	Unbind();
}

void Texture::Buffer2DRGBAData(const Image& image)
{
	assert(target == GL_TEXTURE_2D);
	Bind();
	glTexImage2D(target, 0, internalFormat, static_cast<int32_t>(image.width), static_cast<int32_t>(image.height), 0, format, type, &image.data[0]);
	if (generateMipMap) 
	{
		glGenerateMipmap(target);
	}
	Unbind();
}

void Texture::Buffer2DArrayRGBAData(std::span<const Image> images)
{
	assert(target == GL_TEXTURE_2D_ARRAY);
	assert(!images.empty());
	Bind();

	glTexImage3D(target, 0, internalFormat, static_cast<int32_t>(images[0].width),
		static_cast<int32_t>(images[0].height), images.size(), 0, format, type, nullptr);

	for (size_t i = 0; i < images.size(); i++) 
	{
		glTexSubImage3D(target, 0, 0, 0, i, static_cast<int32_t>(images[i].width),
			static_cast<int32_t>(images[i].height), 1, format, type, images[i].data.data());
	}
	if (generateMipMap) 
	{
		glGenerateMipmap(target);
	}
	Unbind();
}

void Texture::BufferCubeMapRGBAData(std::span<std::shared_ptr<const Image>, 6> images)
{
	assert(target == GL_TEXTURE_CUBE_MAP);
	Bind();

	for (size_t i = 0; i < images.size(); i++) 
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, static_cast<int32_t>(images[i]->width),
			static_cast<int32_t>(images[i]->height), 0, format, type, &images[i]->data[0]);
	}
	if (generateMipMap) 
	{
		glGenerateMipmap(target);
	}
	Unbind();
}

std::shared_ptr<const Texture> Texture::LoadTexture2D(const std::string& name)
{
	std::shared_ptr<const Image> image = AssetManager::instance().loadImage(name);
	if (image == nullptr) return nullptr;
	if (!image) {
		LOG_ERROR("Failed to load texture: {}", name);
	}
	else {
		LOG_INFO("Loaded texture: {} (Width={}, Height={})", name, image->width, image->height);
	}

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, 4);
	texture->Buffer2DRGBAData(*image);
	return texture;
}

std::shared_ptr<const Texture> Texture::LoadTexture2DArray(const std::string& name)
{
	std::shared_ptr<const Image> image = AssetManager::instance().loadImage(name);
	if (image == nullptr) {
		return nullptr;
	}

	// Assume 16x16 splits
	const uint8_t tileWidth = 16;
	const uint8_t tileHeight = 16;
	std::vector<Image> subImages;

	for (uint32_t tileY = 0; tileY < image->height / tileHeight; ++tileY) 
	{
		for (uint32_t tileX = 0; tileX < image->width / tileWidth; ++tileX) 
		{
			subImages.emplace_back(image->subImage({ tileX * tileWidth, tileY * tileHeight }, { tileWidth, tileHeight }));
		}
	}

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(GL_TEXTURE_2D_ARRAY, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, 4);
	texture->Buffer2DArrayRGBAData(subImages);
	return texture;
}

std::shared_ptr<const Texture> Texture::LoadCubeMapTexture(const std::string& name)
{
	std::stringstream parts(name);
	std::array<std::shared_ptr<const Image>, 6> images{};
	std::string imageName;
	AssetManager& assetManager = AssetManager::instance();

	for (int32_t i = 0; i < 6; i++)
	{
		if (!std::getline(parts, imageName, ';'))
		{
			LOG_ERROR("Invalid Cube map name format");
			return nullptr;
		}

		// Prepend ProjectPath to the image name if not already absolute
		if (imageName.find(Settings::ProjectPath) == std::string::npos) {
			imageName = Settings::ProjectPath + "\\" + imageName;
		}

		images[i] = assetManager.loadImage(imageName);
		if (images[i] == nullptr) {
			LOG_ERROR("Failed to load image for cube map: {}", imageName);
			return nullptr;
		}
	}

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(GL_TEXTURE_CUBE_MAP, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	texture->BufferCubeMapRGBAData(images);
	return texture;
}
