#ifndef INPUTGLFW_H
#define INPUTGLFW_H

#include "input.h"

class GLFWwindow;

namespace evnt
{
class InputGLFW : public Input
{
    GLFWwindow * m_owner_window{nullptr};

    static void KeyFuncCallback(GLFWwindow * win, int32_t key, int32_t scancode, int32_t action,
                                int32_t mods);
    static void MouseButtonCallback(GLFWwindow * win, int32_t button, int32_t action, int32_t mods);
    static void MousePositionCallback(GLFWwindow * win, double xpos, double ypos);
    static void MouseWheelCallback(GLFWwindow * win, double xoffset, double yoffset);

public:
    InputGLFW(GLFWwindow * owner);
    ~InputGLFW() override = default;

    void update() override {}
};
}   // namespace evnt

#endif   // INPUTGLFW_H
