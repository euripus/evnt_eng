#include "gltexture.h"

namespace evnt
{
GLTexture::GLTexture(int32_t width, int32_t height, bool isCubemap, int32_t levels,
                     TextureInternalFormat format) :
    ITexture()
{
    m_format = format;
    m_w      = width;
    m_h      = height;
    m_d      = 1;

    if(isCubemap)
        m_type = GL_TEXTURE_CUBE_MAP;
    else
        m_type = GL_TEXTURE_2D;

    glCreateTextures(m_type, 1, &m_id);
    glTextureStorage2D(m_id, levels, TextureInternalFormatToGLTextureInternalFormat(format), m_w, m_h);
    glTextureParameteri(m_id, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(m_id, GL_TEXTURE_MAX_LEVEL, levels - 1);
}

GLTexture::GLTexture(int32_t width, int32_t height, int32_t depth, int32_t levels,
                     TextureInternalFormat format)
{
    m_format = format;
    m_w      = width;
    m_h      = height;
    m_d      = depth;

    m_type = GL_TEXTURE_3D;
    glCreateTextures(m_type, 1, &m_id);
    glTextureStorage3D(m_id, levels, TextureInternalFormatToGLTextureInternalFormat(format), m_w, m_h, m_d);
    glTextureParameteri(m_id, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(m_id, GL_TEXTURE_MAX_LEVEL, levels - 1);
}

GLTexture::GLTexture(int32_t width, int32_t height, int32_t slices, bool isCubemapArray, int32_t levels,
                     TextureInternalFormat format)
{
    if(isCubemapArray)
    {
        slices *= 6;
        m_type = GL_TEXTURE_CUBE_MAP_ARRAY;
    }
    else
        m_type = GL_TEXTURE_2D_ARRAY;

    m_format = format;
    m_w      = width;
    m_h      = height;
    m_d      = slices;

    glCreateTextures(m_type, 1, &m_id);
    glTextureStorage3D(m_id, levels, TextureInternalFormatToGLTextureInternalFormat(format), m_w, m_h, m_d);
    glTextureParameteri(m_id, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(m_id, GL_TEXTURE_MAX_LEVEL, levels - 1);
}

GLTexture::~GLTexture()
{
    destroy();
}

void GLTexture::destroy()
{
    if(m_id != 0)
        glDeleteTextures(1, &m_id);

    m_id = 0;
}

void GLTexture::generateMipmap()
{
    glGenerateTextureMipmap(m_id);
}

void GLTexture::setData(int32_t x, int32_t y, int32_t w, int32_t h, int32_t level, TexturePixelFormat format,
                        TextureDataType type, void const * pixels)
{
    glTextureSubImage2D(m_id, level, x, y, w, h, TexturePixelFormatToGLTexturePixelFormat(format),
                        TextureDataTypeToGLTextureDataType(type), pixels);
}

void GLTexture::setData(int32_t x, int32_t y, int32_t z, int32_t w, int32_t h, int32_t d, int32_t level,
                        TexturePixelFormat format, TextureDataType type, void const * pixels)
{
    glTextureSubImage3D(m_id, level, x, y, z, w, h, d, TexturePixelFormatToGLTexturePixelFormat(format),
                        TextureDataTypeToGLTextureDataType(type), pixels);
}

void GLTexture::copyPixels(ITexture & dest, int32_t sX, int32_t sY, int32_t sZ, int32_t sLevel, int32_t dX,
                           int32_t dY, int32_t dZ, int32_t dLevel, int32_t width, int32_t height,
                           int32_t depth)
{
    glCopyImageSubData(m_id, m_type, sLevel, sX, sY, sZ, dest.getID(), dest.getType(), dLevel, dX, dY, dZ,
                       width, height, depth);
}

void GLTexture::readPixels(int32_t x, int32_t y, int32_t z, int32_t w, int32_t h, int32_t d, int32_t level,
                           TexturePixelFormat format, TextureDataType type, int32_t bufferSize, void * pixels)
{
    glGetTextureSubImage(m_id, level, x, y, z, w, h, d, TexturePixelFormatToGLTexturePixelFormat(format),
                         TextureDataTypeToGLTextureDataType(type), bufferSize, pixels);
}

int32_t GLTexture::bindToSampler(ISampler & sampler)
{
    int32_t unit = sampler.bindToTextureUnit();
    glBindTextureUnit(unit, m_id);
    return unit;
}

std::unique_ptr<Image> GLTexture::createImage(TextureInternalFormat format, int32_t level, bool read,
                                              bool write)
{
    return std::make_unique<GLImage>(sm_images_made++, m_id, level, false, 0,
                                     read ? write ? GL_READ_WRITE : GL_WRITE_ONLY : GL_READ_ONLY, format);
}

std::unique_ptr<Image> GLTexture::createImage(TextureInternalFormat format, int32_t level, bool read,
                                              bool write, int32_t layer)
{
    return std::make_unique<GLImage>(sm_images_made++, m_id, level, true, layer,
                                     read ? write ? GL_READ_WRITE : GL_WRITE_ONLY : GL_READ_ONLY, format);
}

void GLTexture::setMipmapRange(int32_t base, int32_t max)
{
    glTextureParameteri(m_id, GL_TEXTURE_BASE_LEVEL, base);
    glTextureParameteri(m_id, GL_TEXTURE_MAX_LEVEL, max);
}
}   // namespace evnt
