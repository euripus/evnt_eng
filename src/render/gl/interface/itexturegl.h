#ifndef ITEXTUREGL_H
#define ITEXTUREGL_H

#include "../../common/interface/itexture.h"
#include <GL/glew.h>

namespace evnt
{
/// Interface to the texture object implemented in OpenGL
class ITextureGL : public ITexture
{
public:
    /// Returns OpenGL texture handle
    virtual GLuint getGLTextureHandle() = 0;
    /// Returns bind target of the native OpenGL texture
    virtual GLenum getBindTarget() const = 0;
};
}   // namespace evnt

#endif   // ITEXTUREGL_H
