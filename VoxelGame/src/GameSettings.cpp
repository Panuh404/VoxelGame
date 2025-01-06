#include "GamePCH.h"
#include "GameSettings.h"


const char* Settings::WindowName = "Gaim";

const std::string Settings::ProjectPath = std::filesystem::current_path().string();

const std::string Settings::ShaderOpaqueSource =		ProjectPath + R"(\resources\shaders\world_opaque)";
const std::string Settings::ShaderTransparentSource =	ProjectPath + R"(\resources\shaders\world_transparent)";
const std::string Settings::ShaderBlendSource =			ProjectPath + R"(\resources\shaders\world_blend)";
const std::string Settings::ShaderSkyboxSource =		ProjectPath + R"(\resources\shaders\skybox)";
const std::string Settings::ShaderCubeSource =			ProjectPath + R"(\resources\shaders\colored_cube_opaque)";
const std::string Settings::ShaderOutline =				ProjectPath + R"(\resources\shaders\outline)";

const std::string Settings::ShaderIdentity =			ProjectPath + R"(\resources\shaders\identity)";
const std::string Settings::ShaderCrossHair =			ProjectPath + R"(\resources\shaders\crosshair)";
const std::string Settings::ShaderGammaCorrection =		ProjectPath + R"(\resources\shaders\gamma_correction)";
const std::string Settings::ShaderInvert =				ProjectPath + R"(\resources\shaders\invert_effect)";
const std::string Settings::ShaderVignette =			ProjectPath + R"(\resources\shaders\vignette_effect)";


const std::string Settings::TextureAtlasSource = ProjectPath + R"(\resources\textures\default_texture.png)";

const std::string Settings::SkyboxCubeMap =
	"resources/textures/skybox/empty.png;"
	"resources/textures/skybox/empty.png;"
	"resources/textures/skybox/sun.png;"
	"resources/textures/skybox/moon.png;"
	"resources/textures/skybox/empty.png;"
	"resources/textures/skybox/empty.png;";