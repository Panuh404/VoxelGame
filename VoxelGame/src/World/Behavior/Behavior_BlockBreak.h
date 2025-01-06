#pragma once
#include "WorldBehavior.h"
#include "Rendering/ParticleSystem.h"
#include "Rendering/Shader.h"
#include "Geometry/CubeMesh.h"

class BreakParticles : public ParticleSystem
{
	CubeMesh cubeMesh{};
	std::shared_ptr<const ShaderProgram> cubeShader;

public:
	BreakParticles();
	void Render(glm::mat4 MVP) override;
};

class Behavior_BlockBreak : public WorldBehavior
{
	Random random;
	BreakParticles particleSystem;

	void EmitBlockParticle(glm::vec3 pos, glm::vec4 color);

public:
	void OnBlockRemoved(glm::ivec3 blockPos, const BlockData* block, World& world, bool removedByPlayer) override;
	void Update(float dt) override;
	void RenderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum& frustum) override;
};
