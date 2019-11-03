#include "window.h"
#include "../core/core.h"
#include "glfwwindow.h"

namespace evnt
{
std::unique_ptr<Window> Window::CreateMainWindow(std::string const & platform_type, App & owner)
{
    return std::make_unique<GLFWWindow>(owner);
}

Window::Window(App & app) :
    m_owner{app},
    m_title{},
    m_win_size{0, 0},
    m_init_win_size{0, 0},
    m_full_screen{false},
    m_running{false},
    m_MSAA_level{0},
    m_gamma{0.0f},
    m_cursor_visibility{false}
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
