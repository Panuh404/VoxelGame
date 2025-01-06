#include "GamePCH.h"
#include "Behavior_BlockBreak.h"

#include "GameSettings.h"
#include "Scene/AssetManager/AssetManager.h"
#include "Geometry/BlockData.h"

BreakParticles::BreakParticles() : cubeShader(AssetManager::instance().loadShaderProgram(Settings::ShaderCubeSource)) {}

void BreakParticles::Render(glm::mat4 MVP)
{
    cubeShader->Bind();

    for (const auto &particle : particles) 
    {
        cubeShader->SetVec4("color", particle.color);
        cubeShader->SetMat4(
            "MVP", 
            MVP * glm::translate(particle.position) * glm::scale(particle.scale) * glm::rotate(glm::two_pi<float>(), particle.rotation));
        cubeMesh.Render();
    }
}

void Behavior_BlockBreak::EmitBlockParticle(glm::vec3 pos, glm::vec4 color)
{
    particleSystem.Emit({
        .position = pos,
    	.scale = glm::vec3(0.0625),
	    .scaleVelocity = glm::vec3(-0.0625),
	    .scaleVelocityVariation = glm::vec3(.05),
	    .velocity = glm::vec3(1, 0, 1),
	    .velocityVariation = glm::vec3(-2, 1, -2),
	    .gravity = glm::vec3(0, -9.8, 0),
	    .rotation = glm::vec3(0),
	    .angularVelocity = glm::vec3(1, -1, 1),
	    .angularVelocityVariation = glm::vec3(.25, .25, .25),
	    .startColor = color,
	    .endColor = color,
	    .startLifetime = .200,
	    .lifetimeVariation = .200,
	    .lifetime = .200,
    });
}

void Behavior_BlockBreak::OnBlockRemoved(glm::ivec3 blockPos, const BlockData* block, World& world, bool removedByPlayer)
{
    if(!removedByPlayer) return;
    if (block == nullptr || block->type == BlockData::BlockType::Air) return;

    auto color = block->GetColor();
    for (int i = 0; i < 50; ++i) 
    {
        EmitBlockParticle(glm::vec3(blockPos) + random.GetVec3(), color);
    }
}

void Behavior_BlockBreak::Update(float dt)
{
	particleSystem.Update(dt);
}

void Behavior_BlockBreak::RenderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum& frustum)
{
	particleSystem.Render(transform);
}
