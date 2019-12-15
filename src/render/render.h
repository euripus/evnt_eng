#ifndef RENDER_H
#define RENDER_H

#include "./common/devicecaps.h"
#include "./common/interface/idevicecontext.h"
#include "./common/interface/irenderdevice.h"
#include <memory>
#include <vector>

namespace evnt
{
class Render
{
public:
    Render() {}

private:
    DeviceType                                   m_device_type = DeviceType::Undefined;
    std::shared_ptr<IRenderDevice>               m_render_device;
    std::shared_ptr<IDeviceContext>              m_immediate_context;
    std::vector<std::shared_ptr<IDeviceContext>> m_deferred_contexts;
    std::shared_ptr<ISwapChain>                  m_swap_chain;
    HardwareAdapterAttribs                       m_adapter_attribs;
};
}   // namespace evnt
#endif   // RENDER_H
