#include "glbuffer.h"
#include "typeconversions.h"

namespace evnt
{
GLBuffer::GLBuffer(void * data, int32_t tsize, int32_t length, BufferOptions options)
{
    glCreateBuffers(1, &m_id);   // Create CPU handle

    if(length == 0)
        return;
    glNamedBufferStorage(m_id, tsize * length, data, GetEnum(options));   // Initialize buffer store
    m_length = length;
    m_tsize  = tsize;
}

GLBuffer::~GLBuffer()
{
    destroy();
}

void GLBuffer::mapBuffer(int32_t offset, int32_t length, MappingOptions options)
{
    m_mapping_ptr = glMapNamedBufferRange(m_id, offset * m_tsize, length * m_tsize, GetEnum(options));
}

void GLBuffer::unmapBuffer()
{
    glUnmapNamedBuffer(m_id);
}

void GLBuffer::destroy()
{
    if(m_length == 0)
        return;

    if(m_id != 0)
        glDeleteBuffers(1, &m_id);
    m_id = 0;
}

void GLBuffer::copyData(void * data, int32_t offset, int32_t length)
{
    int32_t size = length * m_tsize;
    glNamedBufferSubData(m_id, offset * m_tsize, size, data);
}

void GLBuffer::copyBufferData(int32_t offset, int32_t size, IBuffer & dest, int32_t destOffset)
{
    glCopyNamedBufferSubData(m_id, dest.getID(), offset * m_tsize, destOffset * m_tsize, size * m_tsize);
}

void GLBuffer::flushBuffer(int32_t offset, int32_t length)
{
    glFlushMappedNamedBufferRange(m_id, offset * m_tsize, length * m_tsize);
}

void GLBuffer::copyFromBuffer(void * dest, int32_t offset, int32_t length)
{
    glGetNamedBufferSubData(m_id, offset * m_tsize, length * m_tsize, dest);
}
}   // namespace evnt
