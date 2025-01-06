#pragma once

class AxisPlane
{
	glm::vec3 planeNormal;
	float offsetDirection;
	float planeOffset;
	glm::vec3 rayPosition;
	glm::vec3 rayDirection;
	glm::vec3 hitPosition;

	float hitDistance;

	float Intersect() const;

	float CalculateOffsetDirection(const glm::vec3& direction) const;
	float CalculateStartOffset(const glm::vec3& position, const glm::vec3& direction) const;
	float CalculateHitDistanceToPosition() const { return glm::distance(rayPosition, hitPosition); }
	glm::vec3 CalculateHitPosition() const;

public:
	AxisPlane(glm::vec3 planeNormal, glm::vec3 rayPosition, glm::vec3 rayDirection);
	static std::optional<glm::ivec3> RayHitsToBlockPosition(const glm::vec3& hit1, const glm::vec3& hit2);

	glm::vec3 GetHitPosition() const { return hitPosition; }
	float GetHitDistance() const { return hitDistance; }

	bool operator<(const AxisPlane& other) const { return hitDistance < other.hitDistance; }

	void AdvanceOffset();
};
