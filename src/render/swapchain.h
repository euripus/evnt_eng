#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "framebuffer.h"
#include <cstdint>

namespace evnt
{
class ISwapChain
{
public:
    virtual ~ISwapChain() = default;

    // Presents a rendered image to the user
    virtual void present(uint32_t sync_interval = 1) = 0;

    // Changes the swap chain's back buffer size
    virtual void resize(uint32_t new_width, uint32_t new_height) = 0;

    // Returns the default framebuffer handle
    virtual std::unique_ptr<IFramebuffer> GetDefaultFBO() const = 0;
};
}   // namespace evnt
#endif   // SWAPCHAIN_H
