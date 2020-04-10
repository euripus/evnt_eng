#ifndef RENDER_H
#define RENDER_H

#include "renderdevice.h"
#include "swapchain.h"
#include <memory>
#include <vector>

namespace evnt
{
class Window;

class Render
{
public:
    Render(Window & owner) : m_owner(owner) {}

    void update(double CurrTime, double ElapsedTime);
    void resize(int width, int height);
    void present();

    bool isExtensionsSupported();

    // GPU Data Transfer Interface

    // Fabric method
    static std::unique_ptr<Render> CreateRender(std::string render_type, Window & owner_window);

private:
    using SwapChainPtr = std::unique_ptr<ISwapChain>;

    Window &                      m_owner;
    std::unique_ptr<RenderDevice> mp_device;
    std::vector<SwapChainPtr>     m_swap_chains;
};
}   // namespace evnt
#endif   // RENDER_H
