#ifndef GLSWAPCHAIN_H
#define GLSWAPCHAIN_H

#include "../swapchain.h"
#include "glframebuffer.h"

class GLFWwindow;

namespace evnt
{
class GLSwapChain : public ISwapChain
{
    glm::ivec2   m_win_size{0};
    GLFWwindow * mp_glfw_window{nullptr};

public:
    GLSwapChain(GLFWwindow * glfw_win) : mp_glfw_window(glfw_win) {}

    void present(uint32_t sync_interval = 1) override;

    void resize(uint32_t new_width, uint32_t new_height) override;

    std::unique_ptr<IFramebuffer> GetDefaultFBO() const override { return std::make_unique<GLFramebuffer>(); }
};
}   // namespace evnt

#endif   // GLSWAPCHAIN_H
