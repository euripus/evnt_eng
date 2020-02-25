#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <stdint.h>
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
    BufferStructure(std::vector<Attribute> attributes) : m_attributes(attributes)
    {
        m_total_size = 0;
        for(Attribute att : attributes)
            m_total_size += att.size;
    }

    std::vector<Attribute> GetAttributes() { return m_attributes; }
    int32_t                GetTotalSize() { return m_total_size; }

private:
    std::vector<Attribute> m_attributes;
    int32_t                m_total_size;
};

class VertexArray
{
public:
    VertexArray() {}
    VertexArray(std::vector<BufferStructure> structures);
    void         SetIndexBuffer(Buffer<int> buffer);
    uint32_t     GetID() { return m_id; }
    virtual void Destroy();
    void         SetBuffer(int structure, TBuffer buffer, int offset = 0);
    void         SetAttributeEnabled(int index, bool enabled);
    void         SetStructureEnabled(int structure, bool enabled);

private:
    std::vector<BufferStructure> m_structures;
    int                          m_length   = 0;
    bool                         m_hasIndex = false;
    uint32_t                     m_id       = 0;
};
}   // namespace evnt

#endif   // VERTEXARRAY_H
