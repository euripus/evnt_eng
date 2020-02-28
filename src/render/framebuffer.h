#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "texture.h"

namespace evnt
{
class Framebuffer
{
public:
    Framebuffer();

    virtual void ClearColor(int drawBuffer, glm::vec4 color);
    virtual void ClearDepth(float value);
    virtual void Destroy();

    uint32_t           GetID() { return m_id; }
    static Framebuffer GetDefault() { return Framebuffer(0); }

private:
    Framebuffer(uint32_t id) : m_id(id) {}
    uint32_t m_id;
};
}   // namespace evnt

#endif   // FRAMEBUFFER_H
