#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <cstdint>

namespace evnt
{
class ISwapChain
{
public:
    // Presents a rendered image to the user
    virtual void present(uint32_t sync_interval = 1) = 0;

    // Changes the swap chain's back buffer size
    virtual void resize(uint32_t new_width, uint32_t new_height) = 0;

    // Returns render target view of the current back buffer in the swap chain
};
}   // namespace evnt
#endif   // SWAPCHAIN_H
