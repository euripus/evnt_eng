#include "shaderdescriptor.h"

namespace evnt
{
void ShaderDescriptor::addUniformBuffer(IBuffer * buffer, int32_t bindingPoint, int32_t offset)
{
    m_uniform_buffers.push_back(std::make_tuple(buffer, bindingPoint, offset));
}

void ShaderDescriptor::addShaderStorageBuffer(IBuffer * buffer, int32_t bindingPoint, int32_t offset)
{
    m_ss_buffers.push_back(std::make_tuple(buffer, bindingPoint, offset));
}

void ShaderDescriptor::addAtomicCounterBuffer(IBuffer * buffer, int32_t bindingPoint, int32_t offset)
{
    m_ac_buffers.push_back(std::make_tuple(buffer, bindingPoint, offset));
}
}   // namespace evnt
