#include "window.h"
#include "../core/core.h"

namespace evnt
{
std::unique_ptr<Window> Window::CreateMainWindow(std::string const & platform_type, App & owner)
{
    return std::unique_ptr<Window>{};
}

Window::Window(App & app) : m_owner{app}
{
    Core::instance().registerEvent<evResize>();
    Core::instance().registerEvent<evFullscreen>();

    Core::instance().addFunctor<evResize>(std::bind(&Window::winResize, this, std::placeholders::_1));
    Core::instance().addFunctor<evFullscreen>(std::bind(&Window::winFullscreen, this, std::placeholders::_1));
}

void Window::winResize(glm::ivec2 sz) {}

void Window::winFullscreen(bool is_fullscreen)
{
    fullscreen(is_fullscreen);
}

}   // namespace evnt
