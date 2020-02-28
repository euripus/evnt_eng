#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <vector>

namespace evnt
{
class BufferOptions
{
public:
    bool map_read;
    bool map_write;
    bool map_persistent;
    bool map_coherent;
    bool dynamic_storage;
    bool client_storage;

    BufferOptions(bool mapRead, bool mapWrite, bool mapPersistent, bool mapCoherent, bool dynamic,
                  bool client) :
        map_read(mapRead),
        map_write(mapWrite),
        map_persistent(mapPersistent),
        map_coherent(mapCoherent),
        dynamic_storage(dynamic),
        client_storage(client)
    {}
};

class MappingOptions
{
public:
    bool map_read;
    bool map_write;
    bool map_persistent;
    bool map_coherent;
    bool flush_explicit;
    bool invalidate_range;
    bool invalidate_buffer;
    bool unsynchronized;

    MappingOptions(bool mapRead, bool mapWrite, bool mapPersistent, bool mapCoherent, bool flush,
                   bool invalidateRange, bool invalidateBuffer, bool _unsynchronized) :
        map_read(mapRead),
        map_write(mapWrite),
        map_persistent(mapPersistent),
        map_coherent(mapCoherent),
        flush_explicit(flush),
        invalidate_range(invalidateRange),
        invalidate_buffer(invalidateBuffer),
        unsynchronized(_unsynchronized)
    {}
};

class IBuffer
{
public:
    IBuffer() {}
    // IBuffer(void * data, int32_t tsize, int32_t length, BufferOptions options);
    virtual ~IBuffer() {}

    uint32_t getID() { return m_id; }
    int32_t  getSize() { return m_length; }
    int32_t  getTSize() { return m_tsize; }

    void * getMappedPointer() { return m_mapping_ptr; }
    bool   operator==(const IBuffer & other) const { return m_id == other.m_id; }

    virtual void mapBuffer(int32_t offset, int32_t length, MappingOptions options)                = 0;
    virtual void unmapBuffer()                                                                    = 0;
    virtual void destroy()                                                                        = 0;
    virtual void copyData(void * data, int32_t offset, int32_t length)                            = 0;
    virtual void copyBufferData(int32_t offset, int32_t size, IBuffer & dest, int32_t destOffset) = 0;
    virtual void flushBuffer(int32_t offset, int32_t length)                                      = 0;
    virtual void copyFromBuffer(void * dest, int32_t offset, int32_t length)                      = 0;

protected:
    uint32_t m_id          = 0;
    void *   m_mapping_ptr = nullptr;
    int32_t  m_length      = 0;
    int32_t  m_tsize       = 0;
};

template<class T>
class Buffer
{
    IBuffer * mp_bsinf;

public:
    Buffer(IBuffer * buffer_interface) { mp_bsinf = buffer_interface; }

    uint32_t getID() { return mp_bsinf->getID(); }

    void copyData(std::vector<T> & data, int32_t offset)
    {
        mp_bsinf->copyData(data.data(), static_cast<int32_t>(offset), static_cast<int32_t>(data.size()));
    }

    void copyData(T * data, int32_t offset, int32_t length) { mp_bsinf->copyData(data, offset, length); }

    void copyBufferData(int32_t offset, int32_t length, IBuffer & dest, int32_t destOffset)
    {
        mp_bsinf->copyBufferData(offset, length, dest, destOffset);
    }

    void mapBuffer(int32_t offset, int32_t length, MappingOptions options)
    {
        mp_bsinf->mapBuffer(offset, length, options);
    }

    void flushBuffer(int32_t offset, int32_t length) { mp_bsinf->flushBuffer(offset, length); }

    void copyFromBuffer(T * dest, int32_t offset, int32_t length)
    {
        mp_bsinf->copyFromBuffer(dest, offset, length);
    }

    void copyFromBuffer(std::vector<T> & dest, int32_t destOffset, int32_t offset, int32_t length)
    {
        mp_bsinf->copyFromBuffer(dest.data() + destOffset, offset, length);
    }

    int32_t getLength() { return mp_bsinf->getSize(); }

    T * getMappedPointer() { return static_cast<T *>(mp_bsinf->getMappedPointer()); }
};
}   // namespace evnt

#endif   // BUFFER_H
