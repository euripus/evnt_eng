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

    void addUniformBuffer(IBuffer * buffer, int bindingPoint, int offset = 0);
    void addShaderStorageBuffer(IBuffer * buffer, int bindingPoint, int offset = 0);
    void addAtomicCounterBuffer(IBuffer * buffer, int bindingPoint, int offset = 0);

    virtual void bindDescriptor() = 0;

protected:
    std::vector<std::tuple<IBuffer *, int, int>> m_uniform_buffers;
    std::vector<std::tuple<IBuffer *, int, int>> m_ss_buffers;
    std::vector<std::tuple<IBuffer *, int, int>> m_ac_buffers;
};
}   // namespace evnt
#endif   // SHADERDESCRIPTOR_H
