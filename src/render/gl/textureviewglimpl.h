#ifndef TEXTUREVIEWGLIMPL_H
#define TEXTUREVIEWGLIMPL_H

#include "../common/textureviewbase.h"
#include "interface/itextureviewgl.h"
#include "renderdeviceglimpl.h"

namespace evnt
{
class TextureViewGLImpl final : public TextureViewBase<ITextureViewGL, RenderDeviceGLImpl>
{};
}   // namespace evnt

#endif   // TEXTUREVIEWGLIMPL_H
