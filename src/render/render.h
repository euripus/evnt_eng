#ifndef RENDER_H
#define RENDER_H

#include "../app/window.h"
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
    explicit Render(Window & owner);
    ~Render() = default;

    void update();
    void resize(int32_t width, int32_t height);
    void present(uint32_t sync_interval = 1) { mp_swap_chain->present(sync_interval); }
    bool init();

    bool isExtensionsSupported(std::string const & extension_name)
    {
        return mp_device->isExtensionsSupported(extension_name);
    }

    void addUpdateCommand(std::function<void()> f);

    // Fabric method
    static std::unique_ptr<Render> CreateRender(std::string const & render_type, Window & owner_window);

private:
    Window &                      m_owner;
    std::unique_ptr<RenderDevice> mp_device;
    std::unique_ptr<ISwapChain>   mp_swap_chain;

    // TODO: move to gl render
    ScopedHandle<decltype(Window::evResize)> m_ev_resize_shdl;
    mutable std::mutex                       m_update_queue_mutex;
    std::vector<std::function<void()>>       m_update_queue;
};
}   // namespace evnt
#endif   // RENDER_H
