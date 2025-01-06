#pragma once
#include "Math/Frustum.h"

class World;
struct BlockData;

class WorldBehavior
{
public:
	virtual ~WorldBehavior() = default;

	virtual void OnNewBlock(glm::ivec3 blockPos, const BlockData *block, World &world) {}
	virtual void OnBlockUpdate(glm::ivec3 blockPos, const BlockData *block, World &world) {}
	virtual void OnBlockRemoved(glm::ivec3 blockPos, const BlockData *block, World &world, bool removedByPlayer) {}

	virtual void Update(float dt) {}
	virtual void RenderOpaque(glm::mat4 transform, glm::vec3 playerPos, const Frustum &frustum) {}

};