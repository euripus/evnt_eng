#ifndef GLVERTEXARRAY_H
#define GLVERTEXARRAY_H

#include "../vertexarray.h"

namespace evnt
{
class GLVertexArray : public IVertexArray
{
public:
    GLVertexArray() : IVertexArray() {}
    GLVertexArray(std::vector<BufferStructure> structures);
    ~GLVertexArray() override;

    void setIndexBuffer(Buffer<int32_t> buffer) override;
    void destroy() override;
    void setBuffer(int32_t structure, IBuffer * buffer, int32_t offset = 0) override;
    void setAttributeEnabled(int32_t index, bool enabled) override;
    void setStructureEnabled(int32_t structure, bool enabled) override;
};
}   // namespace evnt

#endif   // GLVERTEXARRAY_H
