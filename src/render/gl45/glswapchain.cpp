#include "glswapchain.h"
#include <GLFW/glfw3.h>

namespace evnt
{
void GLSwapChain::present(uint32_t sync_interval)
{
    glfwSwapBuffers(mp_glfw_window);
}

void GLSwapChain::resize(uint32_t new_width, uint32_t new_height)
{
    m_win_size.x = new_width;
    m_win_size.y = new_height;

    if(m_win_size.x == 0)
    {
        m_win_size.x = 1;
    }
    if(m_win_size.y == 0)
    {
        m_win_size.y = 1;
    }

    glViewport(0, 0, m_win_size.x, m_win_size.y);
}
}   // namespace evnt
