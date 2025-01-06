#pragma once

struct Settings
{
	static const char* WindowName;
	static const std::string ProjectPath;

	static constexpr int32_t WindowWidth = 1920;
	static constexpr int32_t WindowHeight = 1080;

	static constexpr int32_t ChunkHorizontalSize = 16;
	static constexpr int32_t ChunkVerticalSize = 256;

	static constexpr float MovementSpeedMultiplier = 1;
	static constexpr float MouseSensitivity = 0.5;
	static constexpr float DefaultGravity = 46.62f;
	static constexpr float JumpHeightMultiplier = 1;
	static constexpr bool isSurvivalMovement = false;
	static constexpr int32_t WorldSeed = 778;

	static constexpr int32_t ViewDistance = 10;
	static constexpr float TextureAnimationSpeed = 2;

	// Shader Sources
	static const std::string ShaderOpaqueSource;
	static const std::string ShaderTransparentSource;
	static const std::string ShaderBlendSource;
	static const std::string ShaderSkyboxSource;
	static const std::string ShaderCubeSource;
	static const std::string ShaderOutline;

	// Post Process Shaders
	static const std::string ShaderChromaticAberration;
	static const std::string ShaderIdentity;
	static const std::string ShaderCrossHair;
	static const std::string ShaderGammaCorrection;
	static const std::string ShaderInvert;
	static const std::string ShaderVignette;

	// Textures
	static const std::string TextureAtlasSource;
	static const std::string SkyboxCubeMap;

	~Settings() = default;
};