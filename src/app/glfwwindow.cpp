#include "glfwwindow.h"
#include "../core/core.h"
#include "../input/inputglfw.h"
#include "../log/log.h"
#include <GLFW/glfw3.h>

namespace evnt
{
void GLFWWindow::alert(std::string const & title, std::string const & message, AlertType type)
{
    Log::SeverityLevel sl = Log::notification;
    if(type == AlertType::AlertType_Warning)
        sl = Log::warning;
    else if(type == AlertType::AlertType_Error)
        sl = Log::error;

    Log::Log(sl, Log::cstr_log("\"%s\" - %s", title.c_str(), message.c_str()));
}

bool GLFWWindow::init()
{
    int width, height;

    if(!glfwInit())
        return false;

    if(!m_cur_mouse_cursor.isStdShapeCursor())
        if(!m_cur_mouse_cursor.load())
            return false;

    m_base_video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    auto config       = Core::instance().getRootConfig();

    m_running = true;   // Context initialization may change value

    m_full_screen = config.get<bool>("App.Window.Fullscreen");
    m_title       = config.get<std::string>("App.Window.Title");

    GLFWmonitor * mon;
    if(m_full_screen)
    {
        mon    = glfwGetPrimaryMonitor();
        width  = m_base_video_mode->width;
        height = m_base_video_mode->height;
    }
    else
    {
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        mon    = nullptr;
        width  = config.get<int>("App.Window.Size.x");
        height = config.get<int>("App.Window.Size.y");
    }

    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
    glfwWindowHint(GLFW_SAMPLES, m_MSAA_level);
    m_glfw_window = glfwCreateWindow(width, height, "", mon, nullptr);

    if(m_glfw_window == nullptr)
    {
        return false;
    }

    glm::ivec2 sz(width, height);
    winResize(sz);
    glfwSetWindowTitle(m_glfw_window, m_title.c_str());
    setMouseCursor(getMouseCursor());

    m_input_backend = std::make_unique<InputGLFW>(m_glfw_window);

    return true;
}

void GLFWWindow::terminate()
{
    glfwDestroyWindow(m_glfw_window);
    m_glfw_window = nullptr;

    glfwTerminate();
}

void GLFWWindow::update()
{
    m_running = !glfwWindowShouldClose(m_glfw_window);
    if(!m_running)
        m_owner.stop();

    glfwPollEvents();
}

DisplayModes GLFWWindow::getDisplayModes() const
{
    DisplayModes        dms;
    int                 num_modes = 0;
    GLFWvidmode const * modes     = glfwGetVideoModes(glfwGetPrimaryMonitor(), &num_modes);

    for(int i = 0; i < num_modes; ++i)
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

void GLFWWindow::setMouseCursor(MouseCursor cursor)
{
    GLFWcursor * glfw_cursor = nullptr;
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
        glfwSetCursor(m_glfw_window, glfw_cursor);
    }
}

void GLFWWindow::setCursorVisibility(bool type)
{
    if(type)
    {
        glfwSetInputMode(m_glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(m_glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
}
}   // namespace evnt
