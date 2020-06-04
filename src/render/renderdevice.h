#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include "pipeline.h"
#include "shader.h"
#include "shaderconstants.h"
#include "shaderdescriptor.h"
#include <functional>

namespace evnt
{
class RenderDevice
{
protected:
    std::vector<std::function<void()>> m_commands;

    std::vector<ShaderConstants *>  m_constants;
    std::vector<ShaderDescriptor *> m_descriptors;
    std::vector<Pipeline *>         m_pipelines;

public:
    RenderDevice();
    virtual ~RenderDevice() = default;

    void executeCommands();
    void clearCommandBuffer();

    // virtuals
    virtual bool init()                                                          = 0;
    virtual bool isExtensionsSupported(std::string const & extension_name) const = 0;

    virtual void clearColorFramebufferCommand(int32_t attachment, glm::vec4 color) = 0;
    virtual void clearDepthFramebufferCommand(float value)                         = 0;
    virtual void clearAllBuffers()                                                 = 0;

    virtual void bindPixelUnpackBufferCommand(IBuffer * buffer) = 0;
    virtual void bindPixelPackBufferCommand(IBuffer * buffer)   = 0;

    virtual void bindIndirectCommandBufferCommand(IBuffer * buffer) = 0;

    virtual void bindComputeShaderCommand(IShader * shader)           = 0;
    virtual void bindPipelineCommand(Pipeline * pipeline)             = 0;
    virtual void bindConstantsCommand(ShaderConstants * constants)    = 0;
    virtual void bindDescriptorCommand(ShaderDescriptor * descriptor) = 0;

    virtual void drawIndexedCommand(PrimitiveType type, int32_t count, int32_t instances, int32_t baseIndex,
                                    int32_t baseVertex, int32_t baseInstance) = 0;
    virtual void drawCommand(PrimitiveType type, int32_t first, int32_t count, int32_t instances,
                             int32_t baseInstance)                            = 0;
    virtual void drawIndexedIndirectCommand(PrimitiveType type, int32_t offset, int32_t cmdCount,
                                            int32_t stride)                   = 0;
    virtual void drawIndirectCommand(PrimitiveType type, int32_t offset, int32_t cmdCount,
                                     int32_t stride)                          = 0;
    virtual void dispatchCommand(int32_t x, int32_t y, int32_t z)             = 0;

    virtual void memoryBarrier(MemoryBarrierType type) = 0;
    virtual void textureBarrier()                      = 0;
};
}   // namespace evnt
#endif   // RENDERDEVICE_H
