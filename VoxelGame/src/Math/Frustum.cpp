#include "GamePCH.h"
#include "Frustum.h"

// m = ProjectionMatrix * ViewMatrix
Frustum::Frustum(glm::mat4 m)
{
    m = glm::transpose(m);
    m_Planes[Left] = m[3] + m[0];
    m_Planes[Right] = m[3] - m[0];
    m_Planes[Bottom] = m[3] + m[1];
    m_Planes[Top] = m[3] - m[1];
    m_Planes[Near] = m[3] + m[2];
    m_Planes[Far] = m[3] - m[2];

    glm::vec3 crosses[Combinations] = {glm::cross(glm::vec3(m_Planes[Left]), glm::vec3(m_Planes[Right])),
                                       glm::cross(glm::vec3(m_Planes[Left]), glm::vec3(m_Planes[Bottom])),
                                       glm::cross(glm::vec3(m_Planes[Left]), glm::vec3(m_Planes[Top])),
                                       glm::cross(glm::vec3(m_Planes[Left]), glm::vec3(m_Planes[Near])),
                                       glm::cross(glm::vec3(m_Planes[Left]), glm::vec3(m_Planes[Far])),
                                       glm::cross(glm::vec3(m_Planes[Right]), glm::vec3(m_Planes[Bottom])),
                                       glm::cross(glm::vec3(m_Planes[Right]), glm::vec3(m_Planes[Top])),
                                       glm::cross(glm::vec3(m_Planes[Right]), glm::vec3(m_Planes[Near])),
                                       glm::cross(glm::vec3(m_Planes[Right]), glm::vec3(m_Planes[Far])),
                                       glm::cross(glm::vec3(m_Planes[Bottom]), glm::vec3(m_Planes[Top])),
                                       glm::cross(glm::vec3(m_Planes[Bottom]), glm::vec3(m_Planes[Near])),
                                       glm::cross(glm::vec3(m_Planes[Bottom]), glm::vec3(m_Planes[Far])),
                                       glm::cross(glm::vec3(m_Planes[Top]), glm::vec3(m_Planes[Near])),
                                       glm::cross(glm::vec3(m_Planes[Top]), glm::vec3(m_Planes[Far])),
                                       glm::cross(glm::vec3(m_Planes[Near]), glm::vec3(m_Planes[Far])) };

    m_Points[0] = intersection<Left, Bottom, Near>(crosses);
    m_Points[1] = intersection<Left, Top, Near>(crosses);
    m_Points[2] = intersection<Right, Bottom, Near>(crosses);
    m_Points[3] = intersection<Right, Top, Near>(crosses);
    m_Points[4] = intersection<Left, Bottom, Far>(crosses);
    m_Points[5] = intersection<Left, Top, Far>(crosses);
    m_Points[6] = intersection<Right, Bottom, Far>(crosses);
    m_Points[7] = intersection<Right, Top, Far>(crosses);
}

// http://iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
bool Frustum::IsBoxVisible(const glm::vec3& minp, const glm::vec3& maxp) const
{
    // check box outside/inside of frustum
    for (int i = 0; i < Count; i++) {
        if ((glm::dot(m_Planes[i], glm::vec4(minp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_Planes[i], glm::vec4(maxp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_Planes[i], glm::vec4(minp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_Planes[i], glm::vec4(maxp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_Planes[i], glm::vec4(minp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_Planes[i], glm::vec4(maxp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_Planes[i], glm::vec4(minp.x, maxp.y, maxp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_Planes[i], glm::vec4(maxp.x, maxp.y, maxp.z, 1.0f)) < 0.0)) {
            return false;
        }
    }

    // check frustum outside/inside box
    int out;
    out = 0;
    for (int i = 0; i < 8; i++) out += ((m_Points[i].x > maxp.x) ? 1 : 0);
    if (out == 8) return false;
    out = 0;
    for (int i = 0; i < 8; i++) out += ((m_Points[i].x < minp.x) ? 1 : 0);
    if (out == 8) return false;
    out = 0;
    for (int i = 0; i < 8; i++) out += ((m_Points[i].y > maxp.y) ? 1 : 0);
    if (out == 8) return false;
    out = 0;
    for (int i = 0; i < 8; i++) out += ((m_Points[i].y < minp.y) ? 1 : 0);
    if (out == 8)  return false;
    out = 0;
    for (int i = 0; i < 8; i++) out += ((m_Points[i].z > maxp.z) ? 1 : 0);
    if (out == 8) return false;
    out = 0;
    for (int i = 0; i < 8; i++) out += ((m_Points[i].z < minp.z) ? 1 : 0);
    if (out == 8) return false;

    return true;
}

