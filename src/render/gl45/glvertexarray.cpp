#include "glvertexarray.h"
#include <GL/glew.h>

namespace evnt
{
GLVertexArray::GLVertexArray(std::vector<BufferStructure> structures) : IVertexArray(std::move(structures))
{
    glCreateVertexArrays(1, &m_id);

    for(BufferStructure st : structures)
    {
        for(Attribute attrib : st.getAttributes())
        {
            if(attrib.is_float)
                glVertexArrayAttribFormat(m_id, attrib.index, attrib.length, GL_FLOAT, false, attrib.offset);
            else
                glVertexArrayAttribIFormat(m_id, attrib.index, attrib.length, GL_INT, attrib.offset);
            glEnableVertexArrayAttrib(m_id, attrib.index);
            if(attrib.instance_divisor != 0)
                glVertexArrayBindingDivisor(m_id, attrib.index, attrib.instance_divisor);
        }
    }
}

GLVertexArray::~GLVertexArray()
{
    destroy();
}

void GLVertexArray::setIndexBuffer(TBuffer<int32_t> buffer)
{
    glVertexArrayElementBuffer(m_id, buffer.getID());
    m_length    = buffer.getLength();
    m_has_index = true;
}

void GLVertexArray::destroy()
{
    if(m_id != 0)
        glDeleteVertexArrays(1, &m_id);
    m_id = 0;
}

void GLVertexArray::setBuffer(int32_t structure, IBuffer * buffer, int32_t offset)
{
    if(!m_has_index)
        m_length = buffer->getSize() / buffer->getTSize();
    for(Attribute attrib : m_structures[structure].getAttributes())
    {
        GLsizei len = m_structures[structure].getTotalSize();
        glVertexArrayVertexBuffer(m_id, attrib.index, buffer->getID(), offset, len);
    }
}

void GLVertexArray::setAttributeEnabled(int32_t index, bool enabled)
{
    if(enabled)
        glEnableVertexArrayAttrib(m_id, index);
    else
        glDisableVertexArrayAttrib(m_id, index);
}

void GLVertexArray::setStructureEnabled(int32_t structure, bool enabled)
{
    for(Attribute attr : m_structures[structure].getAttributes())
    {
        if(enabled)
            glEnableVertexArrayAttrib(m_id, attr.index);
        else
            glDisableVertexArrayAttrib(m_id, attr.index);
    }
}
}   // namespace evnt
