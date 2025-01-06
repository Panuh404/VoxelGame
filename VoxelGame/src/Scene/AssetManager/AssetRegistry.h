#pragma once
#include "lodepng/lodepng.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Utils/Util.h"

template<typename T>
class AssetRegistry
{
	std::unordered_map<std::string, std::weak_ptr<const T>> registry;
	virtual std::shared_ptr<const T> LoadAsset(const std::string &name) = 0;

public:
	virtual ~AssetRegistry() = default;

	bool HasAsset(const std::string &name) const
	{
		if (!registry.contains(name)) return false;
		return !registry.at(name).expired();
	}

	void Remove(const std::string &name) {
		if (HasAsset(name)) {
			registry[name] = std::shared_ptr<T>();
		}
	}

	std::shared_ptr<const T> Get(const std::string &name)
	{
		if (HasAsset(name)) {
			std::weak_ptr<const T> ref = registry.at(name);
			return ref.lock();
		}

		std::shared_ptr<const T> asset = LoadAsset(name);
		if (asset != nullptr) {
			registry[name] = asset;
		}

		return asset;
	}
};

// TEXT REGISTRY ASSETS
class TextRegistry : public AssetRegistry<std::string>
{
	std::shared_ptr<const std::string> LoadAsset(const std::string& name) override
	{
		LOG_TRACE("Loading Text: {0}", name);
		auto bin = Util::ReadBinaryFile(name);
		if (!bin) LOG_WARN("Failed to load binary: {0}", name);
		return bin;
	}
};

// TEXTURES REGISTRY ASSETS
class TextureRegistry : public AssetRegistry<Texture>
{
	std::shared_ptr<const Texture> LoadAsset(const std::string& name) override
	{
		LOG_TRACE("Loading Texture: {0}", name);
		auto texture = Texture::LoadTexture2D(name);
		if (!texture) LOG_WARN("Failed to load texture: {0}", name);
		return texture;
	}
};

// TEXTURE ARRAY REGISTRY ASSETS
class TextureArrayRegistry : public AssetRegistry<Texture>
{
	std::shared_ptr<const Texture> LoadAsset(const std::string& name) override
	{
		LOG_TRACE("Loading Texture Array: {0}", name);
		auto texture = Texture::LoadTexture2DArray(name);
		if (!texture) LOG_WARN("Failed to load texture array: {0}", name);
		return Texture::LoadTexture2DArray(name);
	}
};

// TEXTURE CUBE MAP REGISTRY ASSETS
class CubeMapRegistry : public AssetRegistry<Texture>
{
	std::shared_ptr<const Texture> LoadAsset(const std::string& name) override
	{
		LOG_TRACE("Loading CubeMap: {0}", name);
		auto texture = Texture::LoadCubeMapTexture(name);
		if (!texture) LOG_WARN("Failed to load CubeMap: {0}", name);
		return Texture::LoadCubeMapTexture(name);
	}
};

// IMAGE REGISTRY ASSETS
class ImageRegistry : public AssetRegistry<Image>
{
	std::shared_ptr<const Image> LoadAsset(const std::string& name) override
	{
		std::shared_ptr<Image> image = std::make_shared<Image>();
		uint32_t error = lodepng::decode(image->data, image->width, image->height, name, LCT_RGBA);

		if (error != 0)
		{
			LOG_ERROR("Failed to read the image file: {0}, message: {1}", name, lodepng_error_text(error));
			return nullptr;
		}
		return image;
	}
};

// SHADER REGISTRY ASSETS
class ShaderRegistry : public AssetRegistry<Shader>
{
	std::shared_ptr<const Shader> LoadAsset(const std::string& name) override
	{
		LOG_TRACE("Loading Shader: {0}", name);
		auto shader = std::make_shared<Shader>(name);
		if (!shader) LOG_WARN("Failed to load shader: {0}", name);
		return shader;
	}
};

// SHADER PROGRAM REGISTRY ASSETS
class ShaderProgramRegistry : public AssetRegistry<ShaderProgram>
{
	std::shared_ptr<const ShaderProgram> LoadAsset(const std::string& name) override
	{
		LOG_TRACE("Loading Shader Program: {0}", name);
		auto shader = std::make_shared<ShaderProgram>(name);
		if (!shader) LOG_WARN("Failed to load shader program: {0}", name);
		return shader;
	}
};