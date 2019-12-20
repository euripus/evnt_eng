#ifndef RENDERDEVICEGLIMPL_H
#define RENDERDEVICEGLIMPL_H

#include "../common/renderdevicebase.h"
#include "./interface/irenderdevicegl.h"
#include "glcontext.h"
#include <mutex>
#include <unordered_map>
#include <unordered_set>

enum class GPU_VENDOR
{
    UNKNOWN,
    INTEL,
    ATI,
    NVIDIA,
    QUALCOMM
};

struct GPUInfo
{
    GPU_VENDOR vendor = GPU_VENDOR::UNKNOWN;
};

namespace evnt
{
class RenderDeviceGLImpl : public RenderDeviceBase<IRenderDeviceGL>
{
public:
    RenderDeviceGLImpl();

    size_t   getCommandQueueCount() const { return 1; }
    uint64_t getCommandQueueMask() const { return uint64_t{1}; }

protected:
    friend class DeviceContextGLImpl;
    friend class TextureBaseGL;
    friend class PipelineStateGLImpl;
    friend class ShaderGLImpl;
    friend class BufferGLImpl;
    friend class TextureViewGLImpl;
    friend class SwapChainGLImpl;
    friend class GLContextState;

    // Must be the first member because its constructor initializes OpenGL
    GLContext m_gl_context;

    std::unordered_set<std::string> m_extension_strings;
    GPUInfo                         m_gpu_info;

    // TexRegionRender m_TexRegionRender;
private:
    bool checkExtension(const char * extension_string);
    void flagSupportedTexFormats();
    void testTextureFormat(TEXTURE_FORMAT tex_format) override;
};
}   // namespace evnt

#endif   // RENDERDEVICEGLIMPL_H
