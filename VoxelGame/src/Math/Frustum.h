#pragma once

class Frustum
{
	enum Planes
	{
		Left = 0,
		Right,
		Bottom,
		Top,
		Near,
		Far,
		Count,
		Combinations = Count * (Count - 1) / 2
	};

	template<Planes i, Planes j>
	struct ij2k {
		enum {
			k = i * (9 - i) / 2 + j - 1
		};
	};

	template<Planes a, Planes b, Planes c>
	glm::vec3 intersection(const glm::vec3* crosses) const;

	glm::vec4 m_Planes[Count];
	glm::vec3 m_Points[8];

public:
	Frustum() {}
	Frustum(glm::mat4 m);

	bool IsBoxVisible(const glm::vec3& minp, const glm::vec3& maxp) const;
};


template<Frustum::Planes a, Frustum::Planes b, Frustum::Planes c>
inline glm::vec3 Frustum::intersection(const glm::vec3* crosses) const
{
	float D = glm::dot(glm::vec3(m_Planes[a]), crosses[ij2k<b, c>::k]);
	glm::vec3 res = glm::mat3(
		crosses[ij2k<b, c>::k], 
		-crosses[ij2k<a, c>::k], 
		crosses[ij2k<a, b>::k]) * glm::vec3(m_Planes[a].w, m_Planes[b].w, m_Planes[c].w);
	return res * (-1.0f / D);
}