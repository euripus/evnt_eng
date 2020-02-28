#ifndef GLBUFFER_H
#define GLBUFFER_H

#include "../buffer.h"

namespace evnt
{
class GLBuffer : public IBuffer
{
public:
    GLBuffer() : IBuffer() {}
    GLBuffer(void * data, int32_t tsize, int32_t length, BufferOptions options);
    ~GLBuffer() override;

    void mapBuffer(int32_t offset, int32_t length, MappingOptions options) override;
    void unmapBuffer() override;
    void destroy() override;
    void copyData(void * data, int32_t offset, int32_t length) override;
    void copyBufferData(int32_t offset, int32_t size, IBuffer & dest, int32_t destOffset) override;
    void flushBuffer(int32_t offset, int32_t length) override;
    void copyFromBuffer(void * dest, int32_t offset, int32_t length) override;
};
}   // namespace evnt

#endif   // GLBUFFER_H
