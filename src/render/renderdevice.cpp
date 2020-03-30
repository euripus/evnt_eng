#include "renderdevice.h"

namespace evnt
{
RenderDevice::RenderDevice()
{
    m_commands.reserve(40);
    m_descriptors.reserve(20);
    m_pipelines.reserve(20);
    m_constants.reserve(20);
}

void RenderDevice::executeCommands()
{
    for(std::function<void()> func : m_commands)
        func();
}

void RenderDevice::clearCommandBuffer()
{
    m_commands.clear();
    m_constants.clear();
    m_descriptors.clear();
    m_pipelines.clear();
}

}   // namespace evnt
