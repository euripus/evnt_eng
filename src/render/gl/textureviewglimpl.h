#ifndef TEXTUREVIEWGLIMPL_H
#define TEXTUREVIEWGLIMPL_H

#include "../common/textureviewbase.h"
#include "globjwrapper.h"
#include "interface/itextureviewgl.h"
#include "renderdeviceglimpl.h"

namespace evnt
{
class TextureViewGLImpl final : public TextureViewBase<ITextureViewGL, RenderDeviceGLImpl>
{
public:
    using TTextureViewBase = TextureViewBase<ITextureViewGL, RenderDeviceGLImpl>;

    const GLObjectWrappers::GLTextureObj & getHandle();
    GLenum                                 getBindTarget();
    void setBindTarget(GLenum view_tex_bind_target) { m_view_tex_bind_target = view_tex_bind_target; }

protected:
    GLObjectWrappers::GLTextureObj m_view_tex_gl_handle;
    GLenum                         m_view_tex_bind_target;
};
}   // namespace evnt

#endif   // TEXTUREVIEWGLIMPL_H
