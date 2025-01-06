#pragma once

#include "GameSettings.h"
#include "PostProcess.h"
#include "Scene/AssetManager/AssetManager.h"

class GammaCorrectionEffect : public PostProcess
{
	float power = 0.85;

public:
	GammaCorrectionEffect(bool enabled) : PostProcess(AssetManager::instance().loadShaderProgram(Settings::ShaderGammaCorrection), enabled) {}

	void Update() override
	{
		shader->SetFloat("power", power);
	}

	void RenderGUI() override
	{
		ImGui::Checkbox("Enable gamma correction", &enabled);

		if (enabled) ImGui::SliderFloat("Gamma correction power", &power, 0.5f, 3.0f);
	}
};
