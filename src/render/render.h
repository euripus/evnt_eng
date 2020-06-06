#ifndef RENDER_H
#define RENDER_H

#include "renderdevice.h"
#include "swapchain.h"
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace evnt
{
class Window;

class Render
{
public:
    Render(Window & owner) : m_owner(owner) {}

    void update();
    void resize(int32_t width, int32_t height) {}
    void present();
    bool init();

    bool isExtensionsSupported(std::string const & extension_name)
    {
        return mp_device->isExtensionsSupported(extension_name);
    }

    // GPU Data Transfer Interface
    void addUpdateCommand(std::function<void()> f)
    {
        std::lock_guard lk(m_update_queue_mutex);
        m_update_queue.push_back(std::move(f));
    }

    // Fabric method
    static std::unique_ptr<Render> CreateRender(std::string const & render_type, Window & owner_window);

private:
    using SwapChainPtr = std::unique_ptr<ISwapChain>;

    Window &                           m_owner;
    std::unique_ptr<RenderDevice>      mp_device;
    std::vector<SwapChainPtr>          m_swap_chains;
    mutable std::mutex                 m_update_queue_mutex;
    std::vector<std::function<void()>> m_update_queue;
};
}   // namespace evnt
#endif   // RENDER_H
