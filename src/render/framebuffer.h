#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "texture.h"

namespace evnt
{
class Framebuffer
{
public:
    Framebuffer()          = default;
    virtual ~Framebuffer() = default;

    virtual void clearColor(int32_t drawBuffer, glm::vec4 color) = 0;
    virtual void clearDepth(float value)                         = 0;
    virtual void destroy()                                       = 0;

    uint32_t getID() const { return m_id; }

    template<typename Derived>
    static std::unique_ptr<Framebuffer> GetDefault()
    {
        return Derived::GetDefault();
    }

protected:
    uint32_t m_id = 0;
};
}   // namespace evnt

#endif   // FRAMEBUFFER_H
