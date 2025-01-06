#pragma once
#include "GameSettings.h"
#include "PostProcess.h"
#include "Application/Window.h"
#include "Scene/AssetManager/AssetManager.h"

class CrosshairEffect : public PostProcess
{
	float crosshairSize = 0.015f;
	float crosshairVerticalWidth = 0.2f;
	float crosshairHorizontalWidth = 0.15f;

public:
	CrosshairEffect(bool enabled) : PostProcess(AssetManager::instance().loadShaderProgram(Settings::ShaderCrossHair), enabled) {}

	void RenderGUI() override
	{
		ImGui::Checkbox("Enable crosshair", &enabled);
		if (enabled) 
		{
			ImGui::SliderFloat("Crosshair size", &crosshairSize, 0.01, 1);
			ImGui::SliderFloat("Crosshair vertical width", &crosshairVerticalWidth, 0.01, 1);
			ImGui::SliderFloat("Crosshair horizontal width", &crosshairHorizontalWidth, 0.01, 1);
		}
	}

	void Update() override
	{
		auto& window = Window::instance();
		auto width = window.GetWindowWidth();
		auto height = window.GetWindowHeight();
		float aspectRatio = width == 0 || height == 0 ? 0 : static_cast<float>(width) / static_cast<float>(height);

		shader->SetFloat("size", crosshairSize);
		shader->SetFloat("verticalWidth", crosshairVerticalWidth);
		shader->SetFloat("horizontalWidth", crosshairHorizontalWidth);
		shader->SetFloat("aspectRatio", aspectRatio);
	}
};
