#include "glframebuffer.h"

namespace evnt
{
GLFramebuffer::GLFramebuffer(std::vector<WrappedTextureView> const & colorAttachments,
                             WrappedTextureView const &              depthStencil) :
    IFramebuffer()
{
    glCreateFramebuffers(1, &m_id);
    int32_t index = 0;
    for(WrappedTextureView colorAttachment : colorAttachments)
    {
        if(colorAttachment.getLayer() == -1)
            glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0 + index, colorAttachment.getID(),
                                      colorAttachment.getLevel());
        else
            glNamedFramebufferTextureLayer(m_id, GL_COLOR_ATTACHMENT0 + index, colorAttachment.getID(),
                                           colorAttachment.getLevel(), colorAttachment.getLayer());
        ++index;
    }
    if(depthStencil.getLayer() == -1)
        glNamedFramebufferTexture(m_id, GL_DEPTH_STENCIL_ATTACHMENT, depthStencil.getID(),
                                  depthStencil.getLevel());
    else
        glNamedFramebufferTextureLayer(m_id, GL_DEPTH_STENCIL_ATTACHMENT, depthStencil.getID(),
                                       depthStencil.getLevel(), depthStencil.getLayer());
}

GLFramebuffer::GLFramebuffer(std::vector<WrappedTextureView> const & colorAttachments,
                             WrappedTextureView const & depth, WrappedTextureView const & stencil,
                             bool hasDepth, bool hasStencil) :
    IFramebuffer()
{
    glCreateFramebuffers(1, &m_id);
    int32_t index = 0;
    for(WrappedTextureView colorAttachment : colorAttachments)
    {
        if(colorAttachment.getLayer() == -1)
            glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0 + index, colorAttachment.getID(),
                                      colorAttachment.getLevel());
        else
            glNamedFramebufferTextureLayer(m_id, GL_COLOR_ATTACHMENT0 + index, colorAttachment.getID(),
                                           colorAttachment.getLevel(), colorAttachment.getLayer());
        ++index;
    }
    if(hasDepth)
    {
        if(depth.getLayer() == -1)
            glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, depth.getID(), depth.getLevel());
        else
            glNamedFramebufferTextureLayer(m_id, GL_DEPTH_ATTACHMENT, depth.getID(), depth.getLevel(),
                                           depth.getLayer());
    }
    if(hasStencil)
    {
        if(stencil.getLayer() == -1)
            glNamedFramebufferTexture(m_id, GL_STENCIL_ATTACHMENT, stencil.getID(), stencil.getLevel());
        else
            glNamedFramebufferTextureLayer(m_id, GL_STENCIL_ATTACHMENT, stencil.getID(), stencil.getLevel(),
                                           stencil.getLayer());
    }
}

GLFramebuffer::~GLFramebuffer()
{
    destroy();
}

void GLFramebuffer::clearColor(int32_t drawBuffer, glm::vec4 color)
{
    if(m_id == 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
        glClearNamedFramebufferfv(m_id, GL_COLOR, drawBuffer, &color[0]);
}

void GLFramebuffer::clearDepth(float value)
{
    glDepthMask(true);
    glClearNamedFramebufferfv(m_id, GL_DEPTH, 0, &value);
}

void GLFramebuffer::destroy()
{
    if(m_id != 0)
        glDeleteFramebuffers(1, &m_id);

    m_id = 0;
}
}   // namespace evnt
