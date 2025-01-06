#pragma once
#include "GameSettings.h"
#include "PostProcess.h"
#include "Scene/AssetManager/AssetManager.h"

class InvertEffect : public PostProcess {
public:
    InvertEffect(bool enabled) : PostProcess(AssetManager::instance().loadShaderProgram(Settings::ShaderInvert), enabled) {}

    void Update() override {}

    void RenderGUI() override {
        ImGui::Checkbox("Enable invert effect", &enabled);
    }
};
