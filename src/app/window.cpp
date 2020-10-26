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
    evResize{},
    evFullscreen{},
    m_owner{app},
    m_title{},
    m_win_size{0, 0},
    m_init_win_size{0, 0},
    m_full_screen{false},
    m_running{false},
    m_MSAA_level{0},
    m_gamma{0.0f},
    m_cursor_visibility{false},
    m_cur_mouse_cursor{}
{
    evResize.bind(std::bind(&Window::winResize, this, std::placeholders::_1, std::placeholders::_2));
    evFullscreen.bind(std::bind(&Window::winFullscreen, this, std::placeholders::_1));
}

void Window::update()
{
    platformUpdate();

    // subsystems update
    mp_input_backend->update();
    mp_renderer->update();
}

void Window::winResize(int32_t w, int32_t h)
{
    int32_t height = h > 0 ? h : 1;
    m_win_size.x   = w;
    m_win_size.y   = height;
}

void Window::winFullscreen(bool is_fullscreen)
{
    fullscreen(is_fullscreen);
}

}   // namespace evnt
