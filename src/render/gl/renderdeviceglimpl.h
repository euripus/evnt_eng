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
    GLContext m_GLContext;

    std::unordered_set<std::string> m_ExtensionStrings;

    std::mutex m_VAOCacheLockFlag;
    // std::unordered_map<GLContext::NativeGLContextType, VAOCache> m_VAOCache;

    std::mutex m_FBOCacheLockFlag;
    // std::unordered_map<GLContext::NativeGLContextType, FBOCache> m_FBOCache;

    GPUInfo m_GPUInfo;

    // TexRegionRender m_TexRegionRender;
};
}   // namespace evnt

#endif   // RENDERDEVICEGLIMPL_H
