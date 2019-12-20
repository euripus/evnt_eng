#include "texturebasegl.h"

namespace evnt
{
TextureBaseGL::TextureBaseGL(RenderDeviceGLImpl * pDeviceGL, const TextureDesc & TexDesc, GLenum BindTarget,
                             const TextureData * pInitData) :
    TTextureBase(pDeviceGL, TexDesc)
{}
}   // namespace evnt
