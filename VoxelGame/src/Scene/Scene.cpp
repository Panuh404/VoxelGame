#include "GamePCH.h"
#include "Scene.h"

#include "Application/Application.h"
#include "Geometry/BlockName.h"
#include "Scene/Player.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/constants.hpp"
#include "Math/WorldRayCast.h"
#include "World/Behavior/Behavior_BlockBreak.h"
#include "World/Behavior/Behavior_LavaParticle.h"

Scene::Scene(const std::string& savePath)
	:	persistence(std::make_shared<Persistence>(savePath)),
		world(std::make_shared<World>(
			persistence,
			std::vector{
				std::static_pointer_cast<WorldBehavior>(std::make_shared<Behavior_LavaParticle>()),
				std::static_pointer_cast<WorldBehavior>(std::make_shared<Behavior_BlockBreak>())
			},Settings::WorldSeed)),
		player(world, persistence)
{
	if (!persistence) 
    {
		LOG_ERROR("Failed to create Persistence!");
		return;
	}
	if (!world) 
    {
		LOG_ERROR("Failed to create World!");
		return;
	}

	auto& app = Application::instance();
	OnResized(app.GetWindowWidth(), app.GetWindowHeight());
	UpdateMouse();
}

void Scene::Update(float dt)
{
    deltaTime = dt;
    player.Update(deltaTime);
    world->Update(player.GetCamera().GetPosition(), deltaTime);
    skybox.Update(projectionMatrix, player.GetCamera().GetViewMatrix(), deltaTime);
}

void Scene::ToggleMenu()
{
    isMenuOpen = !isMenuOpen;
    UpdateMouse();
}

void Scene::UpdateMouse()
{
    if (isMenuOpen) 
    {
        player.ResetMousePosition();
        Window::instance().UnlockMouse();
    }
    else 
    {
        Window::instance().LockMouse();
    }
}

void Scene::Render()
{
    skybox.Render();

    const glm::mat4 mvp = projectionMatrix * player.GetCamera().GetViewMatrix();
    Frustum frustum(mvp);

    const Camera& camera = player.GetCamera();
    const int32_t width = Window::instance().GetWindowWidth();
    const int32_t height = Window::instance().GetWindowHeight();

    static std::shared_ptr<Framebuffer> framebuffer = nullptr;
    if (framebuffer == nullptr || framebuffer->GetWidth() != width || framebuffer->GetHeight() != height) 
    {
        framebuffer = std::make_shared<Framebuffer>(width, height, true, 1);
    }

    Window::instance().GetFramebufferStack()->Push(framebuffer);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    world->RenderOpaque(mvp, camera.GetPosition(), frustum);
    auto opaqueRender = Window::instance().GetFramebufferStack()->Pop();

    world->RenderTransparent(mvp, camera.GetPosition(), frustum, zNear, zFar, opaqueRender);

    if (WorldRayCast ray{ camera.GetPosition(), camera.GetLookDirection(), *world, Player::Reach }) 
    {
        outline.Render(mvp * glm::translate(ray.GetHitTarget().position));
    }

    for (auto& effect : postProcessEffects) 
    {
        effect->Render();
    }
}

void Scene::RenderMenu()
{
    if (ImGui::Begin("Menu")) 
    {
        if (ImGui::Button("Exit", { 120, 20 }))
        {
            glfwSetWindowShouldClose(Window::instance().GetContext(), true);
        }

        float fps = (deltaTime > 0.0f) ? (1.0f / deltaTime) : 0.0f;
        ImGui::Text("FPS: %.1f", fps);             
        ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);

        ImGui::Spacing();

        glm::vec3 position = player.GetCamera().GetPosition();
        ImGui::Text("Player position: x:%f, y:%f, z:%f", position.x, position.y, position.z);
        glm::vec3 lookDirection = player.GetCamera().GetLookDirection();
        ImGui::Text("Player direction: x:%f, y:%f, z:%f", lookDirection.x, lookDirection.y, lookDirection.z);

        ImGui::Spacing();
        ImGui::Spacing();

        bool isSurvival = player.GetSurvivalMovement();
        if (ImGui::Checkbox("Enable \"physics\"", &isSurvival)) 
        {
            player.SetSurvivalMovement(isSurvival);
        }

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Checkbox("Show intermediate textures", &showIntermediateTextures)) 
        {
            Window::instance().GetFramebufferStack()->SetKeepIntermediateTextures(showIntermediateTextures);
        }

        ImGui::Spacing();
        ImGui::Spacing();

        for (auto& effect : postProcessEffects) 
        {
            effect->RenderGUI();

            ImGui::Spacing();
            ImGui::Spacing();
        }

        BlockData::BlockType blockToPlace = player.GetBlockToPlace();
        ImGui::Text("Selected Block: %s", BlockName::BlockTypeToName(blockToPlace));

        ImGui::Spacing();
        ImGui::Spacing();

        BlockName::NameArray names = BlockName::GetBlockNames();
        int32_t selected = BlockName::BlockTypeToIndex(blockToPlace);
        if (ImGui::ListBox("Select a block to place", &selected, &names[0], names.size())) 
        {
            player.SetBlockToPlace(BlockName::BlockNames[selected].first);
        }

        ImGui::Spacing();
        ImGui::Spacing();

        int32_t useOcclusion = world->GetAmbientOcclusion() ? 1 : 0;
        if (ImGui::SliderInt("Use ambient occlusion", &useOcclusion, 0, 1)) 
        {
            world->SetAmbientOcclusion(useOcclusion == 1);
        }

        ImGui::Spacing();

        int32_t distance = world->GetViewDistance();
        if (ImGui::SliderInt("Max render distance", &distance, 1, 64)) 
        {
            world->SetViewDistance(distance);
        }

        ImGui::Spacing();

        float movementSpeed = player.GetMovementSpeedMultiplier();
        if (ImGui::SliderFloat("Player movement speed multiplier", &movementSpeed, 1.0f, 10.0f)) 
        {
            player.SetMovementSpeedMultiplier(movementSpeed);
        }

        ImGui::Spacing();

        float jumpHeight = player.GetJumpHeightMultiplier();
        if (ImGui::SliderFloat("Player jump height multiplier", &jumpHeight, 1.0f, 10.0f)) 
        {
            player.SetJumpHeightMultiplier(jumpHeight);
        }

        ImGui::Spacing();

        float gravity = player.GetGravityConstant() / 10;
        if (ImGui::SliderFloat("Gravity", &gravity, -5, 10.0f)) 
        {
            player.SetGravityConstant(gravity * 10);
        }

        ImGui::Spacing();

        if (ImGui::Button("Reset gravity")) 
        {
            player.SetGravityConstant(Player::DefaultGravity);
        }

        ImGui::Spacing();
        ImGui::Spacing();
        {
            const uint32_t pathLength = 256;
            static char textureAtlasPath[pathLength] = "";
            ImGui::InputText("Save file path", textureAtlasPath, pathLength);
            if (ImGui::Button("Load World")) 
            {
                if (std::filesystem::exists(textureAtlasPath)) 
                {
                    Application::instance().SetScene(std::make_shared<Scene>(textureAtlasPath));
                }
            }
        }
    }

    ImGui::End();
}

void Scene::RenderIntermediateTextures()
{
    if (ImGui::Begin("Intermediate Textures")) 
    {
        for (const auto& texture : Window::instance().GetFramebufferStack()->GetIntermediateTextures()) 
        {
            ImGui::Text("%u", texture->GetId());
            ImGui::Image(static_cast<ImTextureID>(texture->GetId()), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
        }
    }
    ImGui::End();
}

void Scene::RenderGUI()
{
    if (showIntermediateTextures) RenderIntermediateTextures();
    if (isMenuOpen) RenderMenu();
}

void Scene::OnResized(int32_t width, int32_t height)
{
    float aspectRatio = width == 0 || height == 0 ? 0 : static_cast<float>(width) / static_cast<float>(height);
    projectionMatrix = glm::perspective<float>(glm::half_pi<float>(), aspectRatio, zNear, zFar);
}

void Scene::OnKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode)
{
    if (key == GLFW_KEY_ESCAPE) 
    {
        if (action == GLFW_PRESS) 
        {
            ToggleMenu();
        }
        return;
    }
    if (!isMenuOpen) 
    {
        player.OnKeyEvent(key, scancode, action, mode);
    }
}

void Scene::OnMouseButtonEvent(int32_t button, int32_t action, int32_t mods)
{
    if (!isMenuOpen) player.OnMouseButtonEvent(button, action, mods);
    
}

void Scene::OnCursorPositionEvent(double x, double y)
{
    if (!isMenuOpen) player.OnCursorPositionEvent(x, y);
}