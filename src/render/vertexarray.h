#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "buffer.h"
#include <vector>

namespace evnt
{
class Attribute
{
public:
    int32_t index;
    int32_t offset;
    int32_t length;
    int32_t size;
    int32_t instance_divisor;
    bool    is_float;

    Attribute(int32_t _index, int32_t _offset, int32_t _length, int32_t _size, bool isFloat,
              int32_t instanceDivisor = 0) :
        index(_index),
        offset(_offset),
        length(_length),
        size(_size),
        instance_divisor(instanceDivisor),
        is_float(isFloat)
    {}
};

class BufferStructure
{
public:
    BufferStructure(std::vector<Attribute> attributes) : m_attributes(std::move(attributes))
    {
        m_total_size = 0;
        for(Attribute att : attributes)
            m_total_size += att.size;
    }

    std::vector<Attribute> getAttributes() const { return m_attributes; }
    int32_t                getTotalSize() const { return m_total_size; }

private:
    std::vector<Attribute> m_attributes;
    int32_t                m_total_size;
};

class IVertexArray
{
public:
    IVertexArray() {}
    IVertexArray(std::vector<BufferStructure> structures) : IVertexArray()
    {
        std::swap(m_structures, structures);
    }
    virtual ~IVertexArray() {}

    uint32_t getID() const { return m_id; }

    virtual void setIndexBuffer(Buffer<int32_t> buffer)                             = 0;
    virtual void destroy()                                                          = 0;
    virtual void setBuffer(int32_t structure, IBuffer * buffer, int32_t offset = 0) = 0;
    virtual void setAttributeEnabled(int32_t index, bool enabled)                   = 0;
    virtual void setStructureEnabled(int32_t structure, bool enabled)               = 0;

protected:
    std::vector<BufferStructure> m_structures;
    int32_t                      m_length    = 0;
    bool                         m_has_index = false;
    uint32_t                     m_id        = 0;
};
}   // namespace evnt

#endif   // VERTEXARRAY_H
