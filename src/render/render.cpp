#include "render.h"
#include "../core/exception.h"
#include "../app/window.h"
#include "gl45/glrenderdevice.h"

namespace evnt
{
std::unique_ptr<Render> Render::CreateRender(std::string render_type, Window & owner_window)
{
    if(render_type.empty())
    {
        EV_EXCEPT("Render ID not allowed");
    }

    auto ptr = std::make_unique<Render>(owner_window);
    bool is_init{false};

    if(render_type == std::string("gl45"))
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
