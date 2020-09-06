#ifndef GLFWWINDOW_H
#define GLFWWINDOW_H

#include "window.h"

class GLFWvidmode;
class GLFWwindow;

namespace evnt
{
class GLFWWindow : public Window
{
public:
    friend class Render;

    GLFWWindow(App & app) : Window(app), mp_base_video_mode{nullptr}, mp_glfw_window{nullptr} {}
    ~GLFWWindow() override = default;

    void alert(std::string const & title, std::string const & message,
               AlertType type = AlertType::AlertType_Information) override;

    bool init() override;
    void terminate() override;
    void present(uint32_t sync_interval = 1) override;

    void         fullscreen(bool is_fullscreen) override;
    DisplayModes getDisplayModes() const override;

    void adjustGamma() override;

    void setMouseCursor(MouseCursor const & cursor) override;
    void setCursorVisibility(bool type) override;

private:
    bool create(int32_t width, int32_t height);
    void platformUpdate() override;

    GLFWvidmode const * mp_base_video_mode{nullptr};
    GLFWwindow *        mp_glfw_window{nullptr};

    int32_t m_gl_major{1};
    int32_t m_gl_minor{5};
};
}   // namespace evnt

#endif   // GLFWWINDOW_H
