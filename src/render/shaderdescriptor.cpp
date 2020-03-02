#include "shaderdescriptor.h"

namespace evnt
{
void ShaderDescriptor::addUniformBuffer(IBuffer * buffer, int bindingPoint, int offset)
{
    m_uniform_buffers.push_back(std::make_tuple(buffer, bindingPoint, offset));
}

void ShaderDescriptor::addShaderStorageBuffer(IBuffer * buffer, int bindingPoint, int offset)
{
    m_ss_buffers.push_back(std::make_tuple(buffer, bindingPoint, offset));
}

void ShaderDescriptor::addAtomicCounterBuffer(IBuffer * buffer, int bindingPoint, int offset)
{
    m_ac_buffers.push_back(std::make_tuple(buffer, bindingPoint, offset));
}
}   // namespace evnt
