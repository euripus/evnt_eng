#include "glfwwindow.h"
#include "../core/core.h"
#include "../input/inputglfw.h"
#include "../log/debug_messages.h"
#include "../render/render.h"
#include <GLFW/glfw3.h>

namespace evnt
{
// GLFW CallBack
void WindowSizeCallback(GLFWwindow * win, int32_t width, int32_t height)
{
    GLFWWindow * ptr = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(win));
    ptr->evResize(width, height);
}

void GLFWWindow::alert(std::string const & title, std::string const & message, AlertType type)
{
    Log::SeverityLevel sl = Log::notification;
    if(type == AlertType::AlertType_Warning)
        sl = Log::warning;
    else if(type == AlertType::AlertType_Error)
        sl = Log::error;

    Log::Log(sl, Log::cstr_log("\"%s\" - %s", title.c_str(), message.c_str()));
}

bool GLFWWindow::create(int32_t width, int32_t height)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_gl_minor);

    GLFWmonitor * mon;
    if(m_full_screen)
    {
        mon    = glfwGetPrimaryMonitor();
        width  = mp_base_video_mode->width;
        height = mp_base_video_mode->height;
    }
    else
    {
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        mon = nullptr;
    }

    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
    glfwWindowHint(GLFW_SAMPLES, m_MSAA_level);

    GLFWwindow * new_window{nullptr};
    if(mp_glfw_window != nullptr)
    {
        new_window = glfwCreateWindow(width, height, "", mon, mp_glfw_window);
        glfwDestroyWindow(mp_glfw_window);
    }
    else
    {
        new_window = glfwCreateWindow(width, height, "", mon, nullptr);
    }

    mp_glfw_window = new_window;

    if(mp_glfw_window == nullptr)
        EV_EXCEPT("Creating window error!");

    glfwMakeContextCurrent(mp_glfw_window);
    glfwSetWindowTitle(mp_glfw_window, m_title.c_str());
    glfwSetWindowUserPointer(mp_glfw_window, this);
    setMouseCursor(getMouseCursor());

    // subsystem init
    mp_input_backend = std::make_unique<InputGLFW>(mp_glfw_window);
    if(!mp_input_backend->init())
    {
        UNEXPECTED("Input backend init error!");
        return false;
    }

    auto render_type = Core::instance().getRootConfig().get<std::string>("App.Render.RenderType.Type");
    mp_renderer      = Render::CreateRender(render_type, *this);

    if(!mp_renderer->init())
    {
        UNEXPECTED("Render backend init error!");
        return false;
    }

    //    winResize(width, height);
    evResize(width, height);

    return true;
}

bool GLFWWindow::init()
{
    if(!glfwInit())
        return false;

    auto config = Core::instance().getRootConfig();

    m_cursor_visibility = config.get<bool>("App.Window.Cursor.Visibility");
    bool is_std_shape   = config.get<bool>("App.Window.Cursor.IsSTD");
    if(is_std_shape)
        m_cur_mouse_cursor.setStdShape(
            MouseCursor::GetShape(config.get<std::string>("App.Window.Cursor.StdCursorShape")));
    else
        m_cur_mouse_cursor.setFilename(config.get<std::string>("App.Window.Cursor.File"));

    if(!m_cur_mouse_cursor.isStdShapeCursor())
        if(!m_cur_mouse_cursor.load())
            return false;

    mp_base_video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    m_running = true;

    m_full_screen     = config.get<bool>("App.Window.Fullscreen");
    m_title           = config.get<std::string>("App.Window.Title");
    m_MSAA_level      = config.get<int32_t>("App.Window.MSAA");
    m_gl_major        = config.get<int32_t>("App.Render.RenderType.Version.Major");
    m_gl_minor        = config.get<int32_t>("App.Render.RenderType.Version.Minor");
    m_init_win_size.x = config.get<int32_t>("App.Window.Size.x");
    m_init_win_size.y = config.get<int32_t>("App.Window.Size.y");

    return create(m_init_win_size.x, m_init_win_size.y);
}

void GLFWWindow::terminate()
{
    glfwDestroyWindow(mp_glfw_window);
    mp_glfw_window = nullptr;

    glfwTerminate();
}

void GLFWWindow::platformUpdate()
{
    m_running = !glfwWindowShouldClose(mp_glfw_window);
    if(!m_running)
        m_owner.stop();

    glfwPollEvents();
}

void GLFWWindow::present(uint32_t sync_interval)
{
    mp_renderer->present(sync_interval);
}

void GLFWWindow::fullscreen(bool is_fullscreen)
{
    if(m_full_screen == is_fullscreen)
        return;

    m_full_screen = is_fullscreen;
    if(!create(m_init_win_size.x, m_init_win_size.y))
        EV_EXCEPT("Window creation error in fullscreen change!");
}

DisplayModes GLFWWindow::getDisplayModes() const
{
    DisplayModes        dms;
    int32_t             num_modes = 0;
    GLFWvidmode const * modes     = glfwGetVideoModes(glfwGetPrimaryMonitor(), &num_modes);

    for(int32_t i = 0; i < num_modes; ++i)
    {
        DisplayMode dm;
        dm.resolution.x = modes[i].width;
        dm.resolution.y = modes[i].height;

        if(std::find_if(std::begin(dms), std::end(dms),
                        [dm](const DisplayMode & vdm) -> bool {
                            return vdm.resolution.x == dm.resolution.x && vdm.resolution.y == dm.resolution.y;
                        })
           == std::end(dms))
            dms.push_back(dm);
    }

    return dms;
}

void GLFWWindow::adjustGamma()
{
    glfwSetGamma(glfwGetPrimaryMonitor(), m_gamma);
}

void GLFWWindow::setMouseCursor(MouseCursor const & cursor)
{
    static GLFWcursor * old_glfw_cursor = nullptr;
    GLFWcursor *        glfw_cursor     = nullptr;

    if(cursor.isStdShapeCursor())
    {
        MouseCursor::StdCursorShape shp = cursor.getStdShape();
        switch(shp)
        {
            case MouseCursor::StdCursorShape::ARROW: {
                glfw_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
                break;
            }
            case MouseCursor::StdCursorShape::IBEAM: {
                glfw_cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
                break;
            }
            case MouseCursor::StdCursorShape::CROSSHAIR: {
                glfw_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
                break;
            }
            case MouseCursor::StdCursorShape::HAND: {
                glfw_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
                break;
            }
            case MouseCursor::StdCursorShape::HRESIZE: {
                glfw_cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
                break;
            }
            case MouseCursor::StdCursorShape::VRESIZE: {
                glfw_cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
                break;
            }
        }
    }
    else
    {
        GLFWimage image;
        image.width  = cursor.getTexture().width;
        image.height = cursor.getTexture().height;
        image.pixels = cursor.getTexture().data.get();

        glfw_cursor = glfwCreateCursor(&image, cursor.getHotspotX(), cursor.getHotspotY());
    }

    if(glfw_cursor != nullptr)
    {
        if(old_glfw_cursor != nullptr)
            glfwDestroyCursor(old_glfw_cursor);

        glfwSetCursor(mp_glfw_window, glfw_cursor);
        old_glfw_cursor = glfw_cursor;
    }
}

void GLFWWindow::setCursorVisibility(bool type)
{
    if(type)
    {
        glfwSetInputMode(mp_glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(mp_glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
}
}   // namespace evnt
