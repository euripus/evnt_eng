#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>

namespace evnt
{
struct Plane
{
    glm::vec3 m_normal{0.0f};
    float     m_dist{0.0f};

    Plane() = default;

    explicit Plane(const float a, const float b, const float c, const float d)
    {
        m_normal      = glm::vec3(a, b, c);
        float inv_len = 1.0f / m_normal.length();

        m_normal *= inv_len;   // Normalize
        m_dist = d * inv_len;
    }

    Plane(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2)
    {
        m_normal = v1 - v0;
        m_normal = glm::cross(m_normal, v2 - v0);
        m_normal = glm::normalize(m_normal);
        m_dist   = -glm::dot(m_normal, v0);
    }

    // ----------------
    // Other operations
    // ----------------
    float distToPoint(const glm::vec3 & v) const { return glm::dot(m_normal, v) + m_dist; }
};
}   // namespace evnt
#endif   // PLANE_H
