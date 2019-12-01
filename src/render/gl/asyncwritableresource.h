#ifndef ASYNCWRITABLERESOURCE_H
#define ASYNCWRITABLERESOURCE_H

#include <stdint.h>

namespace evnt
{
class AsyncWritableResource
{
public:
    AsyncWritableResource() : m_pending_memory_barriers(0) {}

    void     SetPendingMemoryBarriers(uint32_t barriers) { m_pending_memory_barriers |= barriers; }
    uint32_t GetPendingMemortBarriers() { return m_pending_memory_barriers; }

private:
    friend class GLContextState;
    void ResetPendingMemoryBarriers(uint32_t barriers) { m_pending_memory_barriers = barriers; }
    void ClearPendingMemoryBarriers(uint32_t barriers) { m_pending_memory_barriers &= ~barriers; }

    // Buffer barriers:
    // GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT
    // GL_ELEMENT_ARRAY_BARRIER_BIT
    // GL_UNIFORM_BARRIER_BIT
    // GL_BUFFER_UPDATE_BARRIER_BIT
    // GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT
    // GL_SHADER_STORAGE_BARRIER_BIT

    // Texture barriers:
    // GL_TEXTURE_FETCH_BARRIER_BIT
    // GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
    // GL_PIXEL_BUFFER_BARRIER_BIT
    // GL_TEXTURE_UPDATE_BARRIER_BIT

    // Misc barriers:
    // GL_FRAMEBUFFER_BARRIER_BIT
    // GL_TRANSFORM_FEEDBACK_BARRIER_BIT
    // GL_ATOMIC_COUNTER_BARRIER_BIT
    // GL_QUERY_BUFFER_BARRIER_BIT

    uint32_t m_pending_memory_barriers;
};
}   // namespace evnt

#endif   // ASYNCWRITABLERESOURCE_H
