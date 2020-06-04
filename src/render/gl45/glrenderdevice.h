#ifndef GLRENDERDEVICE_H
#define GLRENDERDEVICE_H

#include "../renderdevice.h"

namespace evnt
{
class GLRenderDevice : public RenderDevice
{
    std::vector<std::string> m_extensions;

public:
    GLRenderDevice();

    bool init() override;
    bool isExtensionsSupported(std::string const & extension_name) const override;

    void clearColorFramebufferCommand(int32_t attachment, glm::vec4 color) override;
    void clearDepthFramebufferCommand(float value) override;
    void clearAllBuffers() override;

    void bindPixelUnpackBufferCommand(IBuffer * buffer) override;
    void bindPixelPackBufferCommand(IBuffer * buffer) override;

    void bindIndirectCommandBufferCommand(IBuffer * buffer) override;

    void bindComputeShaderCommand(IShader * shader) override;
    void bindPipelineCommand(Pipeline * pipeline) override;
    void bindConstantsCommand(ShaderConstants * constants) override;
    void bindDescriptorCommand(ShaderDescriptor * descriptor) override;

    void drawIndexedCommand(PrimitiveType type, int32_t count, int32_t instances, int32_t baseIndex,
                            int32_t baseVertex, int32_t baseInstance) override;
    void drawCommand(PrimitiveType type, int32_t first, int32_t count, int32_t instances,
                     int32_t baseInstance) override;
    void drawIndexedIndirectCommand(PrimitiveType type, int32_t offset, int32_t cmdCount,
                                    int32_t stride) override;
    void drawIndirectCommand(PrimitiveType type, int32_t offset, int32_t cmdCount, int32_t stride) override;
    void dispatchCommand(int32_t x, int32_t y, int32_t z) override;

    void memoryBarrier(MemoryBarrierType type) override;
    void textureBarrier() override;
};
}   // namespace evnt
#endif   // GLRENDERDEVICE_H
