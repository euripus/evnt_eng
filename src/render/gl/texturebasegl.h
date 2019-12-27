#ifndef TEXTUREBASEGL_H
#define TEXTUREBASEGL_H

#include "../common/texturebase.h"
#include "./interface/itexturegl.h"
#include "asyncwritableresource.h"
//#include "devicecontextglimpl.h"
#include "renderdeviceglimpl.h"
#include "textureviewglimpl.h"

namespace evnt
{
/// Base implementation of the evnt::ITextureGL interface
class TextureBaseGL :
    public TextureBase<ITextureGL, RenderDeviceGLImpl, TextureViewGLImpl>,
    public AsyncWritableResource
{
public:
    using TTextureBase = TextureBase<ITextureGL, RenderDeviceGLImpl, TextureViewGLImpl>;
    using ViewImplType = TextureViewGLImpl;

    TextureBaseGL(RenderDeviceGLImpl * p_device_gl, const TextureDesc & tex_desc, GLenum bind_target,
                  const TextureData * p_init_data = nullptr);
    // TextureBaseGL(RenderDeviceGLImpl * pDeviceGL, GLContextState & GLState, const TextureDesc & TexDesc,
    //            GLuint GLTextureHandle, GLenum BindTarget);

    const GLObjectWrappers::GLTextureObj & getGLHandle() const { return m_gl_texture; }
    GLenum                                 getBindTarget() const override final { return m_bind_target; }
    GLenum                                 getGLTexFormat() const { return m_gl_tex_format; }

    void textureMemoryBarrier(uint32_t required_barriers, class GLContextState & gl_context_state);

    virtual void attachToFramebuffer(const struct TextureViewDesc & view_desc, GLenum attachment_point) = 0;

    void copyData(class DeviceContextGLImpl * p_device_ctx_gl, TextureBaseGL * p_src_texture_gl,
                  uint32_t src_mipLevel, uint32_t src_slice, const Box * p_src_box, uint32_t dst_mip_level,
                  uint32_t dst_slice, uint32_t dst_x, uint32_t dst_y, uint32_t dst_z);

    GLuint getGLTextureHandle() override final { return getGLHandle(); }
    void * getNativeHandle() override final
    {
        return reinterpret_cast<void *>(static_cast<size_t>(getGLTextureHandle()));
    }

    virtual void updateData(class GLContextState & ctx_state, uint32_t mip_level, uint32_t slice,
                            const Box & dst_box, const TextureSubResData & subres_data) = 0;

protected:
    void createViewInternal(const struct TextureViewDesc & view_desc, std::shared_ptr<ITextureView> & pp_view,
                            bool is_default_view) override;
    void setDefaultGLParameters();

    GLObjectWrappers::GLTextureObj m_gl_texture;
    const GLenum                   m_bind_target;
    const GLenum                   m_gl_tex_format;
};
}   // namespace evnt
#endif   // TEXTUREBASEGL_H
