#include "render.h"
#include "../app/window.h"
#include "../core/exception.h"
#include "gl45/glrenderdevice.h"

namespace evnt
{
void Render::update()
{
    // execute commands queue in main thread
    if(!m_update_queue.empty())
    {
        for(size_t i = 0; i < m_update_queue.size(); ++i)
        {
            m_update_queue[i]();
        }
        m_update_queue.clear();
    }
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
        // swap chain
        is_init = true;
    }

    if(is_init)
        return ptr;
    else
        EV_EXCEPT("Render creation failed");
}
}   // namespace evnt
