#include "quad2d.h"

namespace evnt
{
Quad2D::Quad2D(IQuad2D & quad) : m_position(quad.m_position), m_size(quad.m_size) {}
}   // namespace evnt
