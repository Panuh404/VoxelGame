#pragma once
#include "WorldBehavior.h"
#include "Rendering/ParticleSystem.h"
#include "Rendering/Shader.h"
#include "Geometry/CubeMesh.h"
#include "Utils/Util.h"

class LavaParticles : public ParticleSystem
{
	CubeMesh cubeMesh;
	std::shared_ptr<const ShaderProgram> cubeShader;

public:
	LavaParticles();
	void Render(glm::mat4 MVP) override;
};


class Behavior_LavaParticle : public WorldBehavior
{
	Random random;
	std::set<glm::ivec3, Util::CompareIVec3> surfaceLavaPositions{};
	LavaParticles particleSystem;
	float emitAttemptFrequency = 0.1;  // seconds
	float timeUntilNextEmit = emitAttemptFrequency;

	void EmitLavaParticles(glm::ivec3 pos);
	void EmitLavaParticle(glm::vec3 pos);

public:
	Behavior_LavaParticle() = default;

	void Update(float dt) override;
	void OnNewBlock(glm::ivec3 blockPos, const BlockData* block, World& world) override;
	void OnBlockUpdate(glm::ivec3 blockPos, const BlockData* block, World& world) override;
	void OnBlockRemoved(glm::ivec3 blockPos, const BlockData* block, World& world, bool removedByPlayer) override;
	void RenderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum& frustum) override;
};