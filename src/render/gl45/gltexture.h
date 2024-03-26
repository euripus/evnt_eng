#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "../texture.h"
#include "typeconversions.h"

namespace evnt
{
class GLImage : public Image
{
public:
    GLImage(int32_t iCreated, int32_t tex, int32_t level, bool layered, uint32_t layer, uint32_t access,
            TextureInternalFormat format) :
        Image(iCreated, tex, level, layered, layer, access, format)
    {}
    ~GLImage() override = default;

    int32_t bindToUnit() override
    {
        auto unit = ImageUnitManager::RefreshUnit(m_icreated);
        glBindImageTexture(unit, m_tex, m_level, m_layered, m_layer, m_access,
                           TextureInternalFormatToGLTextureInternalFormat(m_format));
        return unit;
    }
};

class GLTexture : public ITexture
{
public:
    GLTexture() = default;
    GLTexture(int32_t width, int32_t height, bool isCubemap, int32_t levels, TextureInternalFormat format);
    GLTexture(int32_t width, int32_t height, int32_t depth, int32_t levels, TextureInternalFormat format);
    GLTexture(int32_t width, int32_t height, int32_t slices, bool isCubemapArray, int32_t levels,
              TextureInternalFormat format);

    GLTexture(GLTexture & viewTarget, int32_t baseLevel, int32_t baseSlice, int32_t width, int32_t height,
              bool isCubemap, int32_t levels, TextureInternalFormat format) :
        GLTexture(width, height, isCubemap, levels, format)
    {
        glTextureView(m_id, m_type, viewTarget.getID(),
                      TextureInternalFormatToGLTextureInternalFormat(format), baseLevel, levels, baseSlice,
                      isCubemap ? 6 : 1);
    }
    GLTexture(GLTexture & viewTarget, int32_t baseLevel, int32_t baseSlice, int32_t width, int32_t height,
              int32_t depth, int32_t levels, TextureInternalFormat format) :
        GLTexture(width, height, depth, levels, format)
    {
        glTextureView(m_id, m_type, viewTarget.getID(),
                      TextureInternalFormatToGLTextureInternalFormat(format), baseLevel, levels, baseSlice,
                      depth);
    }
    GLTexture(GLTexture & viewTarget, int32_t baseLevel, int32_t baseSlice, int32_t width, int32_t height,
              int32_t slices, bool isCubemapArray, int32_t levels, TextureInternalFormat format) :
        GLTexture(width, height, slices, isCubemapArray, levels, format)
    {
        glTextureView(m_id, m_type, viewTarget.getID(),
                      TextureInternalFormatToGLTextureInternalFormat(format), baseLevel, levels, baseSlice,
                      m_d);
    }

    ~GLTexture() override;

    void    destroy() override;
    void    generateMipmap() override;
    void    setData(int32_t x, int32_t y, int32_t w, int32_t h, int32_t level, TexturePixelFormat format,
                    TextureDataType type, void const * pixels) override;
    void    setData(int32_t x, int32_t y, int32_t z, int32_t w, int32_t h, int32_t d, int32_t level,
                    TexturePixelFormat format, TextureDataType type, void const * pixels) override;
    void    copyPixels(ITexture & dest, int32_t sX, int32_t sY, int32_t sZ, int32_t sLevel, int32_t dX,
                       int32_t dY, int32_t dZ, int32_t dLevel, int32_t width, int32_t height,
                       int32_t depth) override;
    void    readPixels(int32_t x, int32_t y, int32_t z, int32_t w, int32_t h, int32_t d, int32_t level,
                       TexturePixelFormat format, TextureDataType type, int32_t bufferSize,
                       void * pixels) override;
    int32_t bindToSampler(ISampler & sampler) override;

    std::unique_ptr<Image> createImage(TextureInternalFormat format, int32_t level, bool read,
                                       bool write) override;
    std::unique_ptr<Image> createImage(TextureInternalFormat format, int32_t level, bool read, bool write,
                                       int32_t layer) override;

    void setMipmapRange(int32_t base, int32_t max) override;
};
}   // namespace evnt

#endif   // GLTEXTURE_H
