#ifndef TEXTURE_H
#define TEXTURE_H

#include "sampler.h"
#include <memory>

namespace evnt
{
class ImageUnitManager
{
public:
    static void Initialize(int32_t availableUnits);
    static int  RefreshUnit(int32_t id);

private:
    inline static std::map<int32_t, int32_t> sm_available_units;
    inline static std::queue<int32_t>        sm_age_sort;
};

class Image
{
public:
    Image(int32_t iCreated, int32_t tex, int32_t level, bool layered, uint32_t layer, uint32_t access,
          TextureInternalFormat format) :
        m_tex(tex),
        m_level(level),
        m_layered(layered),
        m_layer(layer),
        m_access(access),
        m_format(format),
        m_icreated(iCreated)
    {}
    virtual ~Image() = default;

    virtual int32_t bindToUnit() = 0;

protected:
    uint32_t              m_tex;
    int32_t               m_level;
    bool                  m_layered;
    uint32_t              m_layer;
    uint32_t              m_access;
    TextureInternalFormat m_format;
    int32_t               m_icreated;
};

class WrappedTextureView
{
public:
    WrappedTextureView() {}
    WrappedTextureView(int32_t layer, int32_t level, TextureInternalFormat format, uint32_t id) :
        m_layer(layer), m_level(level), m_format(format), m_id(id)
    {}

    int32_t               getLayer() const { return m_layer; }
    int32_t               getLevel() const { return m_level; }
    TextureInternalFormat getFormat() const { return m_format; }
    uint32_t              getID() const { return m_id; }

private:
    int32_t               m_layer  = 0;
    int32_t               m_level  = 0;
    TextureInternalFormat m_format = TextureInternalFormat::R8;
    uint32_t              m_id     = 0;
};

class ITexture
{
public:
    ITexture()          = default;
    virtual ~ITexture() = default;

    uint32_t              getID() const { return m_id; }
    uint32_t              getType() const { return m_type; }
    TextureInternalFormat getFormat() const { return m_format; }
    int32_t               getWidth() const { return m_w; }
    int32_t               getHeight() const { return m_h; }
    int32_t               getDepth() const { return m_h; }

    WrappedTextureView createWrappedView(int32_t layer, int32_t level) const;

    virtual void destroy()                                                                     = 0;
    virtual void generateMipmap()                                                              = 0;
    virtual void setData(int32_t x, int32_t y, int32_t w, int32_t h, int32_t level, TexturePixelFormat format,
                         TextureDataType type, const void * pixels)                            = 0;
    virtual void setData(int32_t x, int32_t y, int32_t z, int32_t w, int32_t h, int32_t d, int32_t level,
                         TexturePixelFormat format, TextureDataType type, const void * pixels) = 0;
    virtual void copyPixels(ITexture & dest, int32_t sX, int32_t sY, int32_t sZ, int32_t sLevel, int32_t dX,
                            int32_t dY, int32_t dZ, int32_t dLevel, int32_t width, int32_t height,
                            int32_t depth)                                                     = 0;
    virtual void readPixels(int32_t x, int32_t y, int32_t z, int32_t w, int32_t h, int32_t d, int32_t level,
                            TexturePixelFormat format, TextureDataType type, int32_t bufferSize,
                            void * pixels)                                                     = 0;
    virtual int32_t bindToSampler(ISampler & sampler)                                          = 0;

    virtual std::unique_ptr<Image> createImage(TextureInternalFormat format, int32_t level, bool read,
                                               bool write)                = 0;
    virtual std::unique_ptr<Image> createImage(TextureInternalFormat format, int32_t level, bool read,
                                               bool write, int32_t layer) = 0;

    virtual void setMipmapRange(int32_t base, int32_t max) = 0;

protected:
    int32_t               m_w = 0, m_h = 0, m_d = 0;
    uint32_t              m_id     = 0;
    uint32_t              m_type   = 0;
    TextureInternalFormat m_format = TextureInternalFormat::R8;

    inline static int32_t sm_images_made = 0;
};
}   // namespace evnt
#endif   // TEXTURE_H
