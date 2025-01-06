#pragma once
#include "AssetRegistry.h"

class AssetManager
{
	static AssetManager *instancePtr;

	TextRegistry textRegistry;
	ImageRegistry imageRegistry;
	TextureRegistry textureRegistry;
	TextureArrayRegistry textureArrayRegistry;
	CubeMapRegistry cubeMapRegistry;
	ShaderRegistry shaderRegistry;
	ShaderProgramRegistry shaderProgramRegistry;
	
public:
	AssetManager();
	~AssetManager();

	static AssetManager &instance() { return *instancePtr; }

	void RemoveTextFromRegistry(const std::string &name) { textRegistry.Remove(name); }
	void RemoveImageFromRegistry(const std::string &name) { imageRegistry.Remove(name); }
	void RemoveTextureFromRegistry(const std::string &name) { textureRegistry.Remove(name); }
	void RemoveCubeMapFromRegistry(const std::string &name) { cubeMapRegistry.Remove(name); }
	void RemoveShaderFromRegistry(const std::string &name) { shaderRegistry.Remove(name); }
	void RemoveShaderProgramFromRegistry(const std::string &name) { shaderProgramRegistry.Remove(name); }

	std::shared_ptr<const std::string> loadText(const std::string &name) { return textRegistry.Get(name); }
	std::shared_ptr<const Image> loadImage(const std::string &name) { return imageRegistry.Get(name); }
	std::shared_ptr<const Texture> loadTexture(const std::string &name) { return textureRegistry.Get(name); }
	std::shared_ptr<const Texture> loadTextureArray(const std::string &name) { return textureArrayRegistry.Get(name); }
	std::shared_ptr<const Texture> loadCubeMap(const std::string &name) { return cubeMapRegistry.Get(name); }
	std::shared_ptr<const Shader> loadShader(const std::string &name) { return shaderRegistry.Get(name); }
	std::shared_ptr<const ShaderProgram> loadShaderProgram(const std::string &name) { return shaderProgramRegistry.Get(name); }
};
