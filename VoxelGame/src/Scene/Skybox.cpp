#include "GamePCH.h"
#include "Skybox.h"

#include "GameSettings.h"
#include "AssetManager/AssetManager.h"

Skybox::Skybox()
{
	cubeMap = AssetManager::instance().loadCubeMap(Settings::SkyboxCubeMap);
    shader = AssetManager::instance().loadShaderProgram(Settings::ShaderSkyboxSource);
}

void Skybox::Update(const glm::mat4& projection, const glm::mat4& cameraView, float deltaTime)
{
    rotation += rotationSpeed * deltaTime;
    transform = projection * glm::mat4(glm::mat3(cameraView));
}

void Skybox::Render()
{
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    shader->Bind();
    shader->SetTexture("cubeMap", cubeMap, 1);
    shader->SetMat4("transform", transform * glm::rotate(rotation, glm::vec3(1, 0, 0)));
    vertexArray.RenderIndexed();

    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}
