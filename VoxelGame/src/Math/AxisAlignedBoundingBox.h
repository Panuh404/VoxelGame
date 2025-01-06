#pragma once

struct AxisAlignedBoundingBox
{
	glm::vec3 minPoint;
	glm::vec3 maxPoint;

	explicit constexpr AxisAlignedBoundingBox(const glm::vec3& minPoint, const glm::vec3& maxPoint)
		: minPoint(minPoint), maxPoint(maxPoint) {
	}

	static AxisAlignedBoundingBox PositionFromBlock(const glm::vec3& position)
	{
		return AxisAlignedBoundingBox(position, position + glm::vec3{ 1, 1, 1 });
	}

	bool Intersect(const AxisAlignedBoundingBox& aabb) const
	{
		return
			(GetMinX() <= aabb.GetMaxX() && GetMaxX() >= aabb.GetMinX()) &&
			(GetMinY() <= aabb.GetMaxY() && GetMaxY() >= aabb.GetMinY()) &&
			(GetMinZ() <= aabb.GetMaxZ() && GetMaxZ() >= aabb.GetMinZ());
	}

	glm::vec3 GetMinPoint() const { return minPoint; }
	glm::vec3 GetMaxPoint() const { return maxPoint; }

	float GetMinX() const { return minPoint.x; }
	float GetMinY() const { return minPoint.y; }
	float GetMinZ() const { return minPoint.z; }
	float GetMaxX() const { return maxPoint.x; }
	float GetMaxY() const { return maxPoint.y; }
	float GetMaxZ() const { return maxPoint.z; }

};