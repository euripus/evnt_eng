#ifndef SHADERDESCRIPTOR_H
#define SHADERDESCRIPTOR_H

#include "buffer.h"
#include <tuple>
#include <vector>

namespace evnt
{
class ShaderDescriptor
{
public:
    ShaderDescriptor()          = default;
    virtual ~ShaderDescriptor() = default;

    void addUniformBuffer(IBuffer * buffer, int32_t bindingPoint, int32_t offset = 0);
    void addShaderStorageBuffer(IBuffer * buffer, int32_t bindingPoint, int32_t offset = 0);
    void addAtomicCounterBuffer(IBuffer * buffer, int32_t bindingPoint, int32_t offset = 0);

    virtual void bindDescriptor() = 0;

protected:
    std::vector<std::tuple<IBuffer *, int32_t, int32_t>> m_uniform_buffers;
    std::vector<std::tuple<IBuffer *, int32_t, int32_t>> m_ss_buffers;
    std::vector<std::tuple<IBuffer *, int32_t, int32_t>> m_ac_buffers;
};
}   // namespace evnt
#endif   // SHADERDESCRIPTOR_H
