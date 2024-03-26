#ifndef QUAD2D_H
#define QUAD2D_H

#include <cmath>
#include <glm/glm.hpp>

namespace evnt
{
class IQuad2D;

class Quad2D   // Represents a 2D plane space region
{
public:
    Quad2D() {}
    Quad2D(IQuad2D & quad);
    Quad2D(float x, float y, float w, float h) : m_position(x, y), m_size(w, h) {}
    Quad2D(glm::vec2 pos, glm::vec2 size) : m_position(pos), m_size(size) {}
    Quad2D(glm::vec4 quad) : m_position(quad.x, quad.y), m_size(quad.z, quad.w) {}

    inline Quad2D noOffset() { return Quad2D(glm::vec2(0, 0), m_size); }

    bool intersects(Quad2D other)
    {
        return m_position.x < other.m_position.x + other.m_size.x
               && m_position.x + m_size.x > other.m_position.x && m_position.y + m_size.y > other.m_position.y
               && m_position.y < other.m_position.y + other.m_size.y;
    }

    bool intersects(glm::vec2 pt)
    {
        return pt.x >= m_position.x && pt.y >= m_position.y && pt.x <= m_position.x + m_size.x
               && pt.y <= m_position.y + m_size.y;
    }

    Quad2D inset(float left, float right, float up, float down)
    {
        return Quad2D(m_position.x + left, m_position.y + down, m_size.x - left - right,
                      m_size.y - up - down);
    }

    glm::vec2 getCenter() { return m_position + m_size / 2.f; }

    bool operator==(Quad2D const & other) { return m_position == other.m_position && m_size == other.m_size; }
    bool operator!=(Quad2D const & other) { return m_position != other.m_position || m_size != other.m_size; }

    glm::vec2 m_position;
    glm::vec2 m_size;
};

class IQuad2D
{
public:
    IQuad2D() {}
    IQuad2D(Quad2D & quad) : m_position(glm::floor(quad.m_position)), m_size(glm::ceil(quad.m_size)) {}
    IQuad2D(int32_t x, int32_t y, int32_t w, int32_t h) :
        m_position(glm::round(x), glm::round(y)), m_size(glm::round(w), glm::round(h))
    {}
    IQuad2D(glm::ivec2 pos, glm::ivec2 size) : m_position(pos), m_size(size) {}
    IQuad2D(glm::ivec4 quad) : m_position(quad.x, quad.y), m_size(quad.z, quad.w) {}

    inline IQuad2D noOffset() { return IQuad2D(glm::ivec2(0, 0), m_size); }
    inline IQuad2D mipmapped(int32_t level)
    {
        return IQuad2D(m_position, glm::vec2(m_size) / std::pow(2.0f, float(level)));
    }

    bool intersects(IQuad2D other)
    {
        return m_position.x < other.m_position.x + other.m_size.x
               && m_position.x + m_size.x > other.m_position.x && m_position.y + m_size.y > other.m_position.y
               && m_position.y < other.m_position.y + other.m_size.y;
    }

    glm::vec2 getCenter() { return m_position + m_size / 2; }

    bool operator==(IQuad2D const & other)
    {
        return m_position == other.m_position && m_size == other.m_size;
    }
    bool operator!=(IQuad2D const & other)
    {
        return m_position != other.m_position || m_size != other.m_size;
    }

    glm::ivec2 m_position;
    glm::ivec2 m_size;
};
}   // namespace evnt

#endif   // QUAD2D_H
