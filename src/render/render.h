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

    void update(double CurrTime, double ElapsedTime);
    void resize(int width, int height);
    void present();

    const DeviceCaps & GetDeviceCaps() const { return msp_render_device->getDeviceCaps(); }

private:
    // DeviceCaps                                   m_device_cap;
    std::shared_ptr<IRenderDevice>               msp_render_device;
    std::shared_ptr<IDeviceContext>              msp_immediate_context;
    std::vector<std::shared_ptr<IDeviceContext>> m_deferred_contexts;
    std::shared_ptr<ISwapChain>                  msp_swap_chain;
    HardwareAdapterAttribs                       m_adapter_attribs;
};
}   // namespace evnt
#endif   // RENDER_H
