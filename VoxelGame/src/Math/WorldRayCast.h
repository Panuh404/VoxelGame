#pragma once
#include "Geometry/BlockData.h"
#include "World/World.h"

struct HitTarget
{
	glm::vec3 position;
	BlockData block;
	glm::vec3 neighbor;
	bool hasNeighbor = false;
};

class WorldRayCast
{
	bool success;
	HitTarget hitTarget;

public:
	WorldRayCast(glm::vec3 position, glm::vec3 direction, World& world, float reach);

	bool HasHit() const { return success; }
	HitTarget GetHitTarget() const { return hitTarget; }
	explicit operator bool() const { return HasHit(); }
};
