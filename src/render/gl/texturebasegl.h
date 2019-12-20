#ifndef TEXTUREBASEGL_H
#define TEXTUREBASEGL_H

#include "../common/texturebase.h"
#include "./interface/itexturegl.h"
#include "asyncwritableresource.h"
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

    TextureBaseGL(RenderDeviceGLImpl * pDeviceGL, const TextureDesc & TexDesc, GLenum BindTarget,
                  const TextureData * pInitData = nullptr);
};
}   // namespace evnt
#endif   // TEXTUREBASEGL_H
