#include "glshaderdescriptor.h"
#include "glbuffer.h"
#include <GL/glew.h>

namespace evnt
{
void GLShaderDescriptor::bindDescriptor()
{
    for(std::tuple<IBuffer *, int32_t, int32_t> uniform_buffer : m_uniform_buffers)
    {
        auto [buffer, binding, offset] = uniform_buffer;
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer->getID());
    }

    for(std::tuple<IBuffer *, int32_t, int32_t> ss_buffer : m_ss_buffers)
    {
        auto [buffer, binding, offset] = ss_buffer;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer->getID());
    }

    for(std::tuple<IBuffer *, int32_t, int32_t> ac_buffer : m_ac_buffers)
    {
        auto [buffer, binding, offset] = ac_buffer;
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, binding, buffer->getID());
    }
}
}   // namespace evnt
