#pragma once
#include "GameSettings.h"
#include "PostProcess.h"
#include "Scene/AssetManager/AssetManager.h"

class VignetteEffect : public PostProcess {
    float vignetteIntensity = 2.9;
    float vignetteStart = 2;

public:
    VignetteEffect(bool enabled) : PostProcess(AssetManager::instance().loadShaderProgram(Settings::ShaderVignette), enabled) {}

    void Update() override
	{
        shader->SetFloat("intensity", vignetteIntensity);
        shader->SetFloat("start", vignetteStart);
    }

    void RenderGUI() override
	{
        ImGui::Checkbox("Enable vignette effect", &enabled);

        if (enabled) 
        {
            float invertedIntensity = 4 - vignetteIntensity;
            if (ImGui::SliderFloat("Vignette intensity", &invertedIntensity, 1, 3)) 
            {
                vignetteIntensity = 4 - invertedIntensity;
            }
            ImGui::SliderFloat("Vignette start", &vignetteStart, 0, 3);
        }
    }
};
