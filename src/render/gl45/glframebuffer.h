#ifndef GLFRAMEBUFFER_H
#define GLFRAMEBUFFER_H

#include "../framebuffer.h"
#include "gltexture.h"

namespace evnt
{
class GLFramebuffer : public IFramebuffer
{
public:
    GLFramebuffer() = default;
    GLFramebuffer(const std::vector<WrappedTextureView> & colorAttachments,
                  const WrappedTextureView &              depthStencil);
    GLFramebuffer(const std::vector<WrappedTextureView> & colorAttachments, const WrappedTextureView & depth,
                  const WrappedTextureView & stencil, bool hasDepth, bool hasStencil);
    ~GLFramebuffer() override;

    void clearColor(int32_t drawBuffer, glm::vec4 color) override;
    void clearDepth(float value) override;
    void destroy() override;

    static std::unique_ptr<IFramebuffer> GetDefault() { return std::make_unique<GLFramebuffer>(); }
};
}   // namespace evnt
#endif   // GLFRAMEBUFFER_H
