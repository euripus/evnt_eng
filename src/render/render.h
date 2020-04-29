#ifndef RENDER_H
#define RENDER_H

#include "renderdevice.h"
#include "swapchain.h"
#include <functional>
#include <memory>
#include <vector>

namespace evnt
{
class Window;

class Render
{
public:
    Render(Window & owner) : m_owner(owner) {}

    void update();
    void resize(int width, int height);
    void present();

    bool isExtensionsSupported();

    // GPU Data Transfer Interface
    void addUpdateCommand(std::function<void()> f) { m_update_queue.push_back(std::move(f)); }

    // Fabric method
    static std::unique_ptr<Render> CreateRender(std::string const & render_type, Window & owner_window);

private:
    using SwapChainPtr = std::unique_ptr<ISwapChain>;

    Window &                           m_owner;
    std::unique_ptr<RenderDevice>      mp_device;
    std::vector<SwapChainPtr>          m_swap_chains;
    std::vector<std::function<void()>> m_update_queue;
};
}   // namespace evnt
#endif   // RENDER_H
