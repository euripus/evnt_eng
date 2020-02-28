#ifndef TYPECONVERSIONS_H
#define TYPECONVERSIONS_H

#include "../buffer.h"
#include "../graphics_types.h"
#include <GL/glew.h>

namespace evnt
{
// buffer
GLenum GetEnum(BufferOptions opt);
GLenum GetEnum(MappingOptions opt);

// blend & stencil
GLenum StencilOpToGlStencilOp(StencilOperation stencil_op);
GLenum BlendFactorToGLBlend(BlendFactor bf);
GLenum BlendOperationToGLBlendOp(BlendOperation blend_op);

// sampler
GLenum CompareFuncToGLCompareFunc(ComparisonFunc func);
GLenum WrapModeToGLWrapMode(WrapMode mode);
GLenum MinFilterToGLMinFilter(MinificationFilter fl);
GLenum MagFilterToGLMagFilter(MagnificationFilter fl);
GLenum TexChannelToGLTexChannel(TextureChannel tex_chan);
}   // namespace evnt

#endif   // TYPECONVERSIONS_H
