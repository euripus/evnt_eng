#include "render.h"
#include "../app/glfwwindow.h"
#include "../app/window.h"
#include "../core/exception.h"
#include "gl45/glrenderdevice.h"
#include "gl45/glswapchain.h"

namespace evnt
{
Render::Render(Window & owner) :
    m_owner(owner),
    m_ev_resize_shdl{
        m_owner.evResize,
        m_owner.evResize.bind(std::bind(&Render::resize, this, std::placeholders::_1, std::placeholders::_2))}
{}

void Render::update()
{
    // execute render commands queue in main thread
    {
        std::lock_guard lk(m_update_queue_mutex);
        if(!m_update_queue.empty())
        {
            for(size_t i = 0; i < m_update_queue.size(); ++i)
            {
                m_update_queue[i]();
            }
            m_update_queue.clear();
        }
    }

    mp_device->clearAllBuffers();
    mp_device->executeCommands();
    mp_device->clearCommandBuffer();
}

void Render::resize(int32_t width, int32_t height)
{
    std::function<void()> f = [this, width, height] {
        mp_swap_chain->resize(width, height);
    };
    addUpdateCommand(std::move(f));
}

bool Render::init()
{
    return mp_device->init();
}

void Render::addUpdateCommand(std::function<void()> f)
{
    std::lock_guard lk(m_update_queue_mutex);
    m_update_queue.push_back(std::move(f));
}

std::unique_ptr<Render> Render::CreateRender(std::string const & render_type, Window & owner_window)
{
    if(render_type.empty())
    {
        EV_EXCEPT("Render ID not allowed");
    }

    auto ptr = std::make_unique<Render>(owner_window);
    bool is_init{false};

    if(render_type == std::string("OpenGL"))
    {
        ptr->mp_device = std::make_unique<GLRenderDevice>();
        ptr->mp_swap_chain =
            std::make_unique<GLSwapChain>(dynamic_cast<GLFWWindow &>(owner_window).mp_glfw_window);
        is_init = true;
    }

    if(is_init)
        return ptr;
    else
        EV_EXCEPT("Render creation failed");
}
}   // namespace evnt
