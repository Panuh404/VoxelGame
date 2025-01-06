#pragma once
#include "Player.h"
#include "Skybox.h"
#include "Effects/CrosshairEffect.h"
#include "Effects/GammaCorrectionEffect.h"
#include "Effects/InvertEffect.h"
#include "Effects/PostProcess.h"
#include "Effects/VignetteEffect.h"
#include "Geometry/BlockOutline.h"
#include "World/Persistence.h"

class Scene
{
    std::shared_ptr<Persistence> persistence;
    std::shared_ptr<World> world;

    Skybox skybox;
    Player player;

    const float zNear = 0.1f;
    const float zFar = 1000.0f;
    glm::mat4 projectionMatrix = glm::mat4(1);
    float deltaTime = 1.0f;
    BlockOutline outline{ std::make_shared<CubeMesh>() };
    std::vector<std::shared_ptr<PostProcess>> postProcessEffects = {
    	std::make_shared<CrosshairEffect>(true),
    	std::make_shared<InvertEffect>(false),
    	std::make_shared<VignetteEffect>(true),
    	std::make_shared<GammaCorrectionEffect>(true)
    };

    bool isMenuOpen = false;
    bool showIntermediateTextures = false;

    void ToggleMenu();
    void UpdateMouse();

public:
    explicit Scene(const std::string& savePath);

    void Update(float deltaTime);

    void Render();
    void RenderGUI();
    void RenderMenu();
    void RenderIntermediateTextures();

    void OnResized(int32_t width, int32_t height);
    void OnKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
    void OnCursorPositionEvent(double x, double y);
    void OnMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
};
