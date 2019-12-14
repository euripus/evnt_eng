#ifndef ISWAPCHAIN_H
#define ISWAPCHAIN_H

#include "ideviceobject.h"
#include "itextureview.h"

namespace evnt
{
/// Swap chain description
struct SwapChainDesc : public DeviceObjectAttribs
{
    /// The swap chain width. Default value is 0
    uint32_t width = 0;
    /// The swap chain height. Default value is 0
    uint32_t height = 0;
    /// Back buffer format. Default value is evnt::TEX_FORMAT_RGBA8_UNORM_SRGB
    TEXTURE_FORMAT color_buffer_format = TEX_FORMAT_RGBA8_UNORM_SRGB;
    /// Depth buffer format. Default value is evnt::TEX_FORMAT_D32_FLOAT
    TEXTURE_FORMAT depth_buffer_format = TEX_FORMAT_D32_FLOAT;
    /// Sample count. Default value is 1
    uint32_t samples_count = 1;
    /// Number of buffers int the swap chain
    uint32_t buffer_count = 2;
    /// Default depth value, which is used as optimized depth clear value in D3D12
    float default_depth_value = 1.f;
    /// Default stencil value, which is used as optimized stencil clear value in D3D12
    uint8_t default_stencil_value = 0;

    SwapChainDesc() noexcept {}
    /// Constructor intializes the structure members with default values
    SwapChainDesc(uint32_t _width, uint32_t _height, TEXTURE_FORMAT _color_buffer_format,
                  TEXTURE_FORMAT _depth_buffer_format,
                  uint32_t       _samples_count         = SwapChainDesc{}.samples_count,
                  uint32_t       _buffer_count          = SwapChainDesc{}.buffer_count,
                  float          _default_depth_value   = SwapChainDesc{}.default_depth_value,
                  uint8_t        _default_stencil_value = SwapChainDesc{}.default_stencil_value) :
        width(_width),
        height(_height),
        color_buffer_format(_color_buffer_format),
        depth_buffer_format(_depth_buffer_format),
        samples_count(_samples_count),
        buffer_count(_buffer_count),
        default_depth_value(_default_depth_value),
        default_stencil_value(_default_stencil_value)
    {}
};

/// Swap chain interface
/// The swap chain is created by a platform-dependent function
class ISwapChain : public IDeviceObject
{
public:
    /// Presents a rendered image to the user
    virtual void present(uint32_t SyncInterval = 1) = 0;
    /// Returns the swap chain desctription
    const SwapChainDesc & getDesc() const override = 0;
    /// Changes the swap chain's back buffer size
    /// \param [in] NewWidth - New swap chain width, in pixels
    /// \param [in] NewHeight - New swap chain height, in pixels
    virtual void resize(uint32_t NewWidth, uint32_t NewHeight) = 0;
    /// Sets fullscreen mode (only supported on Win32 platform)
    virtual void setFullscreenMode(const DisplayModeAttribs & DisplayMode) = 0;
    /// Sets windowed mode (only supported on Win32 platform)
    virtual void setWindowedMode() = 0;
    /// Returns render target view of the current back buffer in the swap chain
    /// \note For Direct3D12 and Vulkan backends, the function returns
    /// different pointer for every offscreen buffer in the swap chain
    /// (flipped by every call to ISwapChain::Present()). For Direct3D11
    /// backend it always returns the same pointer. For OpenGL/GLES backends
    /// the method returns null.
    ///
    /// The method does *NOT* call AddRef() on the returned interface,
    /// so Release() must not be called.
    virtual ITextureView * getCurrentBackBufferRTV() = 0;
    /// Returns depth-stencil view of the depth buffer
    /// The method does *NOT* call AddRef() on the returned interface,
    /// so Release() must not be called.
    virtual ITextureView * getDepthBufferDSV() = 0;
};
}   // namespace evnt

#endif   // ISWAPCHAIN_H
