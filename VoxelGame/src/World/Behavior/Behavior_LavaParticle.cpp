#include "GamePCH.h"
#include "Behavior_LavaParticle.h"

#include "GameSettings.h"
#include "Scene/AssetManager/AssetManager.h"
#include "Geometry/BlockData.h"
#include "World/World.h"

LavaParticles::LavaParticles() : cubeShader(AssetManager::instance().loadShaderProgram(Settings::ShaderCubeSource)) {}

void LavaParticles::Render(glm::mat4 MVP)
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

void Behavior_LavaParticle::Update(float dt)
{
	timeUntilNextEmit -= dt;
	if (timeUntilNextEmit <= 0)
	{
		timeUntilNextEmit = emitAttemptFrequency;
		for (auto lavaPosition : surfaceLavaPositions)
		{
			if (random.GetFloat() < 0.99f) continue;
			EmitLavaParticles(lavaPosition);
		}
	}
	particleSystem.Update(dt);
}


void Behavior_LavaParticle::EmitLavaParticles(glm::ivec3 pos)
{
	float particlesToEmit = glm::log2(random.GetFloat() * 8 + 0.01);

	for (float i = 0; i < particlesToEmit; ++i)
	{
		const glm::vec2 posOnBlock = random.GetVec2();
		EmitLavaParticle(glm::vec3(pos) + glm::vec3(posOnBlock.x, 1, posOnBlock.y));
	}
}

void Behavior_LavaParticle::EmitLavaParticle(glm::vec3 pos)
{
	particleSystem.Emit({
		.position = pos,
		.scale = glm::vec3(0.0625),
		.scaleVelocity = glm::vec3(-0.0625),
		.scaleVelocityVariation = glm::vec3(.05),
		.velocity = glm::vec3(2, 3, 2),
		.velocityVariation = glm::vec3(-4, 1, -4),
		.gravity = glm::vec3(0, -9.8, 0),
		.rotation = glm::vec3(0),
		.angularVelocity = glm::vec3(1, -1, 1),
		.angularVelocityVariation = glm::vec3(.25, .25, .25),
		.startColor = glm::vec4(.8, 0, 0, 1),
		.endColor = glm::vec4(.4, 0, 0, 1),
		.startLifetime = .5,
		.lifetimeVariation = .25,
		.lifetime = .5
	});
}

void Behavior_LavaParticle::OnNewBlock(glm::ivec3 blockPos, const BlockData* block, World& world)
{
	if (block == nullptr || block->type != BlockData::BlockType::Lava) return;

	auto blockAbove = world.GetBlockAtIfLoaded(blockPos + glm::ivec3(0, 1, 0));
	if (blockAbove != nullptr && blockAbove->type == BlockData::BlockType::Air) surfaceLavaPositions.emplace(blockPos);
}

void Behavior_LavaParticle::OnBlockUpdate(glm::ivec3 blockPos, const BlockData* block, World& world)
{
	if (block == nullptr || block->type != BlockData::BlockType::Lava) return;

	auto blockAbove = world.GetBlockAtIfLoaded(blockPos + glm::ivec3(0, 1, 0));
	if (blockAbove == nullptr || blockAbove->type == BlockData::BlockType::Air) surfaceLavaPositions.emplace(blockPos);
	else surfaceLavaPositions.erase(blockPos);
}

void Behavior_LavaParticle::OnBlockRemoved(glm::ivec3 blockPos, const BlockData* block, World& world, bool removedByPlayer)
{
	if (block == nullptr || block->type != BlockData::BlockType::Lava) return;
	surfaceLavaPositions.erase(blockPos);
}

void Behavior_LavaParticle::RenderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum& frustum)
{
	particleSystem.Render(transform);
}

