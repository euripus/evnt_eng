#include "typeconversions.h"
#include "../itextureview.h"
#include <unordered_map>

namespace evnt
{
class FormatToGLInternalTexFormatMap
{
public:
    FormatToGLInternalTexFormatMap()
    {
        // http://www.opengl.org/wiki/Image_Format
        m_FmtToGLFmtMap[TEX_FORMAT_UNKNOWN] = 0;

        m_FmtToGLFmtMap[TEX_FORMAT_RGBA32_TYPELESS] = GL_RGBA32F;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA32_FLOAT]    = GL_RGBA32F;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA32_UINT]     = GL_RGBA32UI;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA32_SINT]     = GL_RGBA32I;

        m_FmtToGLFmtMap[TEX_FORMAT_RGB32_TYPELESS] = GL_RGB32F;
        m_FmtToGLFmtMap[TEX_FORMAT_RGB32_FLOAT]    = GL_RGB32F;
        m_FmtToGLFmtMap[TEX_FORMAT_RGB32_UINT]     = GL_RGB32UI;
        m_FmtToGLFmtMap[TEX_FORMAT_RGB32_SINT]     = GL_RGB32I;

        m_FmtToGLFmtMap[TEX_FORMAT_RGBA16_TYPELESS] = GL_RGBA16F;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA16_FLOAT]    = GL_RGBA16F;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA16_UNORM]    = GL_RGBA16;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA16_UINT]     = GL_RGBA16UI;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA16_SNORM]    = GL_RGBA16_SNORM;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA16_SINT]     = GL_RGBA16I;

        m_FmtToGLFmtMap[TEX_FORMAT_RG32_TYPELESS] = GL_RG32F;
        m_FmtToGLFmtMap[TEX_FORMAT_RG32_FLOAT]    = GL_RG32F;
        m_FmtToGLFmtMap[TEX_FORMAT_RG32_UINT]     = GL_RG32UI;
        m_FmtToGLFmtMap[TEX_FORMAT_RG32_SINT]     = GL_RG32I;

        m_FmtToGLFmtMap[TEX_FORMAT_R32G8X24_TYPELESS]        = GL_DEPTH32F_STENCIL8;
        m_FmtToGLFmtMap[TEX_FORMAT_D32_FLOAT_S8X24_UINT]     = GL_DEPTH32F_STENCIL8;
        m_FmtToGLFmtMap[TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS] = GL_DEPTH32F_STENCIL8;
        m_FmtToGLFmtMap[TEX_FORMAT_X32_TYPELESS_G8X24_UINT]  = 0;   // GL_DEPTH32F_STENCIL8;

        m_FmtToGLFmtMap[TEX_FORMAT_RGB10A2_TYPELESS] = GL_RGB10_A2;
        m_FmtToGLFmtMap[TEX_FORMAT_RGB10A2_UNORM]    = GL_RGB10_A2;
        m_FmtToGLFmtMap[TEX_FORMAT_RGB10A2_UINT]     = GL_RGB10_A2UI;
        m_FmtToGLFmtMap[TEX_FORMAT_R11G11B10_FLOAT]  = GL_R11F_G11F_B10F;

        m_FmtToGLFmtMap[TEX_FORMAT_RGBA8_TYPELESS]   = GL_RGBA8;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA8_UNORM]      = GL_RGBA8;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA8_UNORM_SRGB] = GL_SRGB8_ALPHA8;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA8_UINT]       = GL_RGBA8UI;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA8_SNORM]      = GL_RGBA8_SNORM;
        m_FmtToGLFmtMap[TEX_FORMAT_RGBA8_SINT]       = GL_RGBA8I;

        m_FmtToGLFmtMap[TEX_FORMAT_RG16_TYPELESS] = GL_RG16F;
        m_FmtToGLFmtMap[TEX_FORMAT_RG16_FLOAT]    = GL_RG16F;
        m_FmtToGLFmtMap[TEX_FORMAT_RG16_UNORM]    = GL_RG16;
        m_FmtToGLFmtMap[TEX_FORMAT_RG16_UINT]     = GL_RG16UI;
        m_FmtToGLFmtMap[TEX_FORMAT_RG16_SNORM]    = GL_RG16_SNORM;
        m_FmtToGLFmtMap[TEX_FORMAT_RG16_SINT]     = GL_RG16I;

        m_FmtToGLFmtMap[TEX_FORMAT_R32_TYPELESS] = GL_R32F;
        m_FmtToGLFmtMap[TEX_FORMAT_D32_FLOAT]    = GL_DEPTH_COMPONENT32F;
        m_FmtToGLFmtMap[TEX_FORMAT_R32_FLOAT]    = GL_R32F;
        m_FmtToGLFmtMap[TEX_FORMAT_R32_UINT]     = GL_R32UI;
        m_FmtToGLFmtMap[TEX_FORMAT_R32_SINT]     = GL_R32I;

        m_FmtToGLFmtMap[TEX_FORMAT_R24G8_TYPELESS]        = GL_DEPTH24_STENCIL8;
        m_FmtToGLFmtMap[TEX_FORMAT_D24_UNORM_S8_UINT]     = GL_DEPTH24_STENCIL8;
        m_FmtToGLFmtMap[TEX_FORMAT_R24_UNORM_X8_TYPELESS] = GL_DEPTH24_STENCIL8;
        m_FmtToGLFmtMap[TEX_FORMAT_X24_TYPELESS_G8_UINT]  = 0;   // GL_DEPTH24_STENCIL8;

        m_FmtToGLFmtMap[TEX_FORMAT_RG8_TYPELESS] = GL_RG8;
        m_FmtToGLFmtMap[TEX_FORMAT_RG8_UNORM]    = GL_RG8;
        m_FmtToGLFmtMap[TEX_FORMAT_RG8_UINT]     = GL_RG8UI;
        m_FmtToGLFmtMap[TEX_FORMAT_RG8_SNORM]    = GL_RG8_SNORM;
        m_FmtToGLFmtMap[TEX_FORMAT_RG8_SINT]     = GL_RG8I;

        m_FmtToGLFmtMap[TEX_FORMAT_R16_TYPELESS] = GL_R16F;
        m_FmtToGLFmtMap[TEX_FORMAT_R16_FLOAT]    = GL_R16F;
        m_FmtToGLFmtMap[TEX_FORMAT_D16_UNORM]    = GL_DEPTH_COMPONENT16;
        m_FmtToGLFmtMap[TEX_FORMAT_R16_UNORM]    = GL_R16;
        m_FmtToGLFmtMap[TEX_FORMAT_R16_UINT]     = GL_R16UI;
        m_FmtToGLFmtMap[TEX_FORMAT_R16_SNORM]    = GL_R16_SNORM;
        m_FmtToGLFmtMap[TEX_FORMAT_R16_SINT]     = GL_R16I;

        m_FmtToGLFmtMap[TEX_FORMAT_R8_TYPELESS] = GL_R8;
        m_FmtToGLFmtMap[TEX_FORMAT_R8_UNORM]    = GL_R8;
        m_FmtToGLFmtMap[TEX_FORMAT_R8_UINT]     = GL_R8UI;
        m_FmtToGLFmtMap[TEX_FORMAT_R8_SNORM]    = GL_R8_SNORM;
        m_FmtToGLFmtMap[TEX_FORMAT_R8_SINT]     = GL_R8I;
        m_FmtToGLFmtMap[TEX_FORMAT_A8_UNORM]    = 0;

        m_FmtToGLFmtMap[TEX_FORMAT_R1_UNORM] = 0;

        m_FmtToGLFmtMap[TEX_FORMAT_RGB9E5_SHAREDEXP] = GL_RGB9_E5;
        m_FmtToGLFmtMap[TEX_FORMAT_RG8_B8G8_UNORM]   = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_G8R8_G8B8_UNORM]  = 0;

        // http://www.g-truc.net/post-0335.html
        // http://renderingpipeline.com/2012/07/texture-compression/
        m_FmtToGLFmtMap[TEX_FORMAT_BC1_TYPELESS] = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC1_UNORM] =
            GL_COMPRESSED_RGB_S3TC_DXT1_EXT;   // GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC1_UNORM_SRGB] =
            GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;   // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC2_TYPELESS]               = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC2_UNORM]                  = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC2_UNORM_SRGB]             = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC3_TYPELESS]               = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC3_UNORM]                  = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC3_UNORM_SRGB]             = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC4_TYPELESS]               = GL_COMPRESSED_RED_RGTC1;
        m_FmtToGLFmtMap[TEX_FORMAT_BC4_UNORM]                  = GL_COMPRESSED_RED_RGTC1;
        m_FmtToGLFmtMap[TEX_FORMAT_BC4_SNORM]                  = GL_COMPRESSED_SIGNED_RED_RGTC1;
        m_FmtToGLFmtMap[TEX_FORMAT_BC5_TYPELESS]               = GL_COMPRESSED_RG_RGTC2;
        m_FmtToGLFmtMap[TEX_FORMAT_BC5_UNORM]                  = GL_COMPRESSED_RG_RGTC2;
        m_FmtToGLFmtMap[TEX_FORMAT_BC5_SNORM]                  = GL_COMPRESSED_SIGNED_RG_RGTC2;
        m_FmtToGLFmtMap[TEX_FORMAT_B5G6R5_UNORM]               = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_B5G5R5A1_UNORM]             = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_BGRA8_UNORM]                = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_BGRX8_UNORM]                = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM] = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_BGRA8_TYPELESS]             = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_BGRA8_UNORM_SRGB]           = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_BGRX8_TYPELESS]             = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_BGRX8_UNORM_SRGB]           = 0;
        m_FmtToGLFmtMap[TEX_FORMAT_BC6H_TYPELESS]              = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC6H_UF16]                  = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC6H_SF16]                  = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
        m_FmtToGLFmtMap[TEX_FORMAT_BC7_TYPELESS]               = GL_COMPRESSED_RGBA_BPTC_UNORM;
        m_FmtToGLFmtMap[TEX_FORMAT_BC7_UNORM]                  = GL_COMPRESSED_RGBA_BPTC_UNORM;
        m_FmtToGLFmtMap[TEX_FORMAT_BC7_UNORM_SRGB]             = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
    }

    GLenum operator[](TEXTURE_FORMAT TexFormat) const
    {
        assert(TexFormat < std::size(m_FmtToGLFmtMap));
        return m_FmtToGLFmtMap[TexFormat];
    }

private:
    GLenum m_FmtToGLFmtMap[TEX_FORMAT_NUM_FORMATS] = {0};
};

GLenum TexFormatToGLInternalTexFormat(TEXTURE_FORMAT TexFormat, uint32_t BindFlags)
{
    static const FormatToGLInternalTexFormatMap FormatMap;
    if(TexFormat >= TEX_FORMAT_UNKNOWN && TexFormat < TEX_FORMAT_NUM_FORMATS)
    {
        auto GLFormat = FormatMap[TexFormat];
        if(BindFlags != 0)
            GLFormat = CorrectGLTexFormat(GLFormat, BindFlags);
        return GLFormat;
    }
    else
    {
        UNEXPECTED("Texture format (", int{TexFormat}, ") out of allowed range [0, ",
                   int{TEX_FORMAT_NUM_FORMATS} - 1, "]");
        return 0;
    }
}

class InternalTexFormatToTexFormatMap
{
public:
    InternalTexFormatToTexFormatMap()
    {
        for(TEXTURE_FORMAT TexFmt = TEX_FORMAT_UNKNOWN; TexFmt < TEX_FORMAT_NUM_FORMATS;
            TexFmt                = static_cast<TEXTURE_FORMAT>(static_cast<int>(TexFmt) + 1))
        {
            /*auto ComponentType = GetTextureFormatAttribs(TexFmt).ComponentType;
            if(ComponentType == COMPONENT_TYPE_UNDEFINED || ComponentType == COMPONENT_TYPE_DEPTH_STENCIL
               || TexFmt == TEX_FORMAT_RGB10A2_TYPELESS || TexFmt == TEX_FORMAT_BC1_TYPELESS
               || TexFmt == TEX_FORMAT_BC2_TYPELESS || TexFmt == TEX_FORMAT_BC3_TYPELESS
               || TexFmt == TEX_FORMAT_BC4_TYPELESS || TexFmt == TEX_FORMAT_BC5_TYPELESS
               || TexFmt == TEX_FORMAT_BC6H_TYPELESS || TexFmt == TEX_FORMAT_BC7_TYPELESS)
                continue;   // Skip typeless and depth-stencil formats*/
            auto GlTexFormat = TexFormatToGLInternalTexFormat(TexFmt);
            if(GlTexFormat != 0)
            {
                assert(m_FormatMap.find(GlTexFormat) == m_FormatMap.end());
                m_FormatMap[GlTexFormat] = TexFmt;
            }
            m_FormatMap[TexFormatToGLInternalTexFormat(TEX_FORMAT_D32_FLOAT_S8X24_UINT)] =
                TEX_FORMAT_D32_FLOAT_S8X24_UINT;
            m_FormatMap[TexFormatToGLInternalTexFormat(TEX_FORMAT_D24_UNORM_S8_UINT)] =
                TEX_FORMAT_D24_UNORM_S8_UINT;
        }
    }

    TEXTURE_FORMAT operator[](GLenum GlFormat) const
    {
        auto formatIt = m_FormatMap.find(GlFormat);
        if(formatIt != m_FormatMap.end())
        {
            assert(GlFormat == TexFormatToGLInternalTexFormat(formatIt->second));
            return formatIt->second;
        }
        else
        {
            UNEXPECTED("Unknown GL format");
            return TEX_FORMAT_UNKNOWN;
        }
    }

private:
    std::unordered_map<GLenum, TEXTURE_FORMAT> m_FormatMap;
};

TEXTURE_FORMAT GLInternalTexFormatToTexFormat(GLenum GlFormat)
{
    static const InternalTexFormatToTexFormatMap FormatMap;
    return FormatMap[GlFormat];
}

GLenum CorrectGLTexFormat(GLenum GLTexFormat, uint32_t BindFlags)
{
    if(BindFlags & BIND_DEPTH_STENCIL)
    {
        if(GLTexFormat == GL_R32F)
            GLTexFormat = GL_DEPTH_COMPONENT32F;
        if(GLTexFormat == GL_R16)
            GLTexFormat = GL_DEPTH_COMPONENT16;
    }
    return GLTexFormat;
}

NativePixelAttribs GetNativePixelTransferAttribs(TEXTURE_FORMAT TexFormat)
{
    // http://www.opengl.org/wiki/Pixel_Transfer

    static bool               bAttribsMapIntialized = false;
    static NativePixelAttribs FmtToGLPixelFmt[TEX_FORMAT_NUM_FORMATS];
    if(!bAttribsMapIntialized)
    {
        // http://www.opengl.org/wiki/Image_Format
        FmtToGLPixelFmt[TEX_FORMAT_UNKNOWN] = NativePixelAttribs();

        FmtToGLPixelFmt[TEX_FORMAT_RGBA32_TYPELESS] = NativePixelAttribs(GL_RGBA, GL_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA32_FLOAT]    = NativePixelAttribs(GL_RGBA, GL_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA32_UINT]     = NativePixelAttribs(GL_RGBA_INTEGER, GL_UNSIGNED_INT);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA32_SINT]     = NativePixelAttribs(GL_RGBA_INTEGER, GL_INT);

        FmtToGLPixelFmt[TEX_FORMAT_RGB32_TYPELESS] = NativePixelAttribs(GL_RGB, GL_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RGB32_FLOAT]    = NativePixelAttribs(GL_RGB, GL_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RGB32_UINT]     = NativePixelAttribs(GL_RGB_INTEGER, GL_UNSIGNED_INT);
        FmtToGLPixelFmt[TEX_FORMAT_RGB32_SINT]     = NativePixelAttribs(GL_RGB_INTEGER, GL_INT);

        FmtToGLPixelFmt[TEX_FORMAT_RGBA16_TYPELESS] = NativePixelAttribs(GL_RGBA, GL_HALF_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA16_FLOAT]    = NativePixelAttribs(GL_RGBA, GL_HALF_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA16_UNORM]    = NativePixelAttribs(GL_RGBA, GL_UNSIGNED_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA16_UINT]     = NativePixelAttribs(GL_RGBA_INTEGER, GL_UNSIGNED_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA16_SNORM]    = NativePixelAttribs(GL_RGBA, GL_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA16_SINT]     = NativePixelAttribs(GL_RGBA_INTEGER, GL_SHORT);

        FmtToGLPixelFmt[TEX_FORMAT_RG32_TYPELESS] = NativePixelAttribs(GL_RG, GL_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RG32_FLOAT]    = NativePixelAttribs(GL_RG, GL_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RG32_UINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_UNSIGNED_INT);
        FmtToGLPixelFmt[TEX_FORMAT_RG32_SINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_INT);

        FmtToGLPixelFmt[TEX_FORMAT_R32G8X24_TYPELESS] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
        FmtToGLPixelFmt[TEX_FORMAT_D32_FLOAT_S8X24_UINT] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
        FmtToGLPixelFmt[TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
        FmtToGLPixelFmt[TEX_FORMAT_X32_TYPELESS_G8X24_UINT] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);

        // Components are normally packed with the first component in the most significant
        // bits of the bitfield, and successive component occupying progressively less
        // significant locations.Types whose token names end with _REV reverse the component
        // packing order from least to most significant locations.
        FmtToGLPixelFmt[TEX_FORMAT_RGB10A2_TYPELESS] =
            NativePixelAttribs(GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV);
        FmtToGLPixelFmt[TEX_FORMAT_RGB10A2_UNORM] =
            NativePixelAttribs(GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV);
        FmtToGLPixelFmt[TEX_FORMAT_RGB10A2_UINT] =
            NativePixelAttribs(GL_RGBA_INTEGER, GL_UNSIGNED_INT_2_10_10_10_REV);
        FmtToGLPixelFmt[TEX_FORMAT_R11G11B10_FLOAT] =
            NativePixelAttribs(GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV);

        FmtToGLPixelFmt[TEX_FORMAT_RGBA8_TYPELESS]   = NativePixelAttribs(GL_RGBA, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA8_UNORM]      = NativePixelAttribs(GL_RGBA, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA8_UNORM_SRGB] = NativePixelAttribs(GL_RGBA, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA8_UINT]       = NativePixelAttribs(GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA8_SNORM]      = NativePixelAttribs(GL_RGBA, GL_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_RGBA8_SINT]       = NativePixelAttribs(GL_RGBA_INTEGER, GL_BYTE);

        FmtToGLPixelFmt[TEX_FORMAT_RG16_TYPELESS] = NativePixelAttribs(GL_RG, GL_HALF_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RG16_FLOAT]    = NativePixelAttribs(GL_RG, GL_HALF_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_RG16_UNORM]    = NativePixelAttribs(GL_RG, GL_UNSIGNED_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_RG16_UINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_UNSIGNED_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_RG16_SNORM]    = NativePixelAttribs(GL_RG, GL_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_RG16_SINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_SHORT);

        FmtToGLPixelFmt[TEX_FORMAT_R32_TYPELESS] = NativePixelAttribs(GL_RED, GL_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_D32_FLOAT]    = NativePixelAttribs(GL_DEPTH_COMPONENT, GL_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_R32_FLOAT]    = NativePixelAttribs(GL_RED, GL_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_R32_UINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_UNSIGNED_INT);
        FmtToGLPixelFmt[TEX_FORMAT_R32_SINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_INT);

        FmtToGLPixelFmt[TEX_FORMAT_R24G8_TYPELESS] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
        FmtToGLPixelFmt[TEX_FORMAT_D24_UNORM_S8_UINT] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
        FmtToGLPixelFmt[TEX_FORMAT_R24_UNORM_X8_TYPELESS] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
        FmtToGLPixelFmt[TEX_FORMAT_X24_TYPELESS_G8_UINT] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);

        FmtToGLPixelFmt[TEX_FORMAT_RG8_TYPELESS] = NativePixelAttribs(GL_RG, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_RG8_UNORM]    = NativePixelAttribs(GL_RG, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_RG8_UINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_RG8_SNORM]    = NativePixelAttribs(GL_RG, GL_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_RG8_SINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_BYTE);

        FmtToGLPixelFmt[TEX_FORMAT_R16_TYPELESS] = NativePixelAttribs(GL_RED, GL_HALF_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_R16_FLOAT]    = NativePixelAttribs(GL_RED, GL_HALF_FLOAT);
        FmtToGLPixelFmt[TEX_FORMAT_D16_UNORM]    = NativePixelAttribs(GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_R16_UNORM]    = NativePixelAttribs(GL_RED, GL_UNSIGNED_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_R16_UINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_UNSIGNED_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_R16_SNORM]    = NativePixelAttribs(GL_RED, GL_SHORT);
        FmtToGLPixelFmt[TEX_FORMAT_R16_SINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_SHORT);

        FmtToGLPixelFmt[TEX_FORMAT_R8_TYPELESS] = NativePixelAttribs(GL_RED, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_R8_UNORM]    = NativePixelAttribs(GL_RED, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_R8_UINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_UNSIGNED_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_R8_SNORM]    = NativePixelAttribs(GL_RED, GL_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_R8_SINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_BYTE);
        FmtToGLPixelFmt[TEX_FORMAT_A8_UNORM]    = NativePixelAttribs();

        FmtToGLPixelFmt[TEX_FORMAT_R1_UNORM] = NativePixelAttribs();

        FmtToGLPixelFmt[TEX_FORMAT_RGB9E5_SHAREDEXP] =
            NativePixelAttribs(GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV);
        FmtToGLPixelFmt[TEX_FORMAT_RG8_B8G8_UNORM]  = NativePixelAttribs();
        FmtToGLPixelFmt[TEX_FORMAT_G8R8_G8B8_UNORM] = NativePixelAttribs();

        // http://www.g-truc.net/post-0335.html
        // http://renderingpipeline.com/2012/07/texture-compression/
        FmtToGLPixelFmt[TEX_FORMAT_BC1_TYPELESS]   = NativePixelAttribs(GL_RGB, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC1_UNORM]      = NativePixelAttribs(GL_RGB, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC1_UNORM_SRGB] = NativePixelAttribs(GL_RGB, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC2_TYPELESS]   = NativePixelAttribs(GL_RGBA, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC2_UNORM]      = NativePixelAttribs(GL_RGBA, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC2_UNORM_SRGB] = NativePixelAttribs(GL_RGBA, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC3_TYPELESS]   = NativePixelAttribs(GL_RGBA, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC3_UNORM]      = NativePixelAttribs(GL_RGBA, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC3_UNORM_SRGB] = NativePixelAttribs(GL_RGBA, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC4_TYPELESS]   = NativePixelAttribs(GL_RED, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC4_UNORM]      = NativePixelAttribs(GL_RED, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC4_SNORM]      = NativePixelAttribs(GL_RED, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC5_TYPELESS]   = NativePixelAttribs(GL_RG, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC5_UNORM]      = NativePixelAttribs(GL_RG, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC5_SNORM]      = NativePixelAttribs(GL_RG, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_B5G6R5_UNORM]   = NativePixelAttribs(GL_RGB, GL_UNSIGNED_SHORT_5_6_5_REV);
        FmtToGLPixelFmt[TEX_FORMAT_B5G5R5A1_UNORM] =
            NativePixelAttribs(GL_RGB, GL_UNSIGNED_SHORT_1_5_5_5_REV);
        FmtToGLPixelFmt[TEX_FORMAT_BGRA8_UNORM]                = NativePixelAttribs();
        FmtToGLPixelFmt[TEX_FORMAT_BGRX8_UNORM]                = NativePixelAttribs();
        FmtToGLPixelFmt[TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM] = NativePixelAttribs();
        FmtToGLPixelFmt[TEX_FORMAT_BGRA8_TYPELESS]             = NativePixelAttribs();
        FmtToGLPixelFmt[TEX_FORMAT_BGRA8_UNORM_SRGB]           = NativePixelAttribs();
        FmtToGLPixelFmt[TEX_FORMAT_BGRX8_TYPELESS]             = NativePixelAttribs();
        FmtToGLPixelFmt[TEX_FORMAT_BGRX8_UNORM_SRGB]           = NativePixelAttribs();
        FmtToGLPixelFmt[TEX_FORMAT_BC6H_TYPELESS]              = NativePixelAttribs(GL_RGB, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC6H_UF16]                  = NativePixelAttribs(GL_RGB, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC6H_SF16]                  = NativePixelAttribs(GL_RGB, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC7_TYPELESS]               = NativePixelAttribs(GL_RGBA, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC7_UNORM]                  = NativePixelAttribs(GL_RGBA, 0, true);
        FmtToGLPixelFmt[TEX_FORMAT_BC7_UNORM_SRGB]             = NativePixelAttribs(GL_RGBA, 0, true);

        bAttribsMapIntialized = true;
    }

    if(TexFormat > TEX_FORMAT_UNKNOWN && TexFormat < TEX_FORMAT_NUM_FORMATS)
    {
        return FmtToGLPixelFmt[TexFormat];
    }
    else
    {
        UNEXPECTED("Texture format (", int{TexFormat}, ") is out of allowed range [1, ",
                   int{TEX_FORMAT_NUM_FORMATS} - 1, "]");
        return FmtToGLPixelFmt[0];
    }
}

GLenum AccessFlags2GLAccess(uint32_t UAVAccessFlags)
{
    static GLenum AccessFlags2GLAccessMap[UAV_ACCESS_FLAG_READ_WRITE + 1] = {};
    static bool   bIsInit                                                 = false;
    if(!bIsInit)
    {
        AccessFlags2GLAccessMap[UAV_ACCESS_FLAG_READ]       = GL_READ_ONLY;
        AccessFlags2GLAccessMap[UAV_ACCESS_FLAG_WRITE]      = GL_WRITE_ONLY;
        AccessFlags2GLAccessMap[UAV_ACCESS_FLAG_READ_WRITE] = GL_READ_WRITE;
        bIsInit                                             = true;
    }

    auto GLAccess = AccessFlags2GLAccessMap[UAVAccessFlags & UAV_ACCESS_FLAG_READ_WRITE];
    return GLAccess;
}

GLenum TypeToGLTexFormat(VALUE_TYPE ValType, uint32_t NumComponents, bool bIsNormalized)
{
    switch(ValType)
    {
        case VT_FLOAT16: {
            assert(!bIsNormalized);   // Floating point formats cannot be normalized
            switch(NumComponents)
            {
                case 1:
                    return GL_R16F;
                case 2:
                    return GL_RG16F;
                case 4:
                    return GL_RGBA16F;
                default:
                    UNEXPECTED("Unusupported number of components");
                    return 0;
            }
        }

        case VT_FLOAT32: {
            assert(!bIsNormalized);   // Floating point formats cannot be normalized
            switch(NumComponents)
            {
                case 1:
                    return GL_R32F;
                case 2:
                    return GL_RG32F;
                case 3:
                    return GL_RGB32F;
                case 4:
                    return GL_RGBA32F;
                default:
                    UNEXPECTED("Unusupported number of components");
                    return 0;
            }
        }

        case VT_INT32: {
            assert(!bIsNormalized);   // 32-bit UNORM formats are not supported. Use R32_FLOAT instead
            switch(NumComponents)
            {
                case 1:
                    return GL_R32I;
                case 2:
                    return GL_RG32I;
                case 3:
                    return GL_RGB32I;
                case 4:
                    return GL_RGBA32I;
                default:
                    UNEXPECTED("Unusupported number of components");
                    return 0;
            }
        }

        case VT_UINT32: {
            assert(!bIsNormalized);   // 32-bit UNORM formats are not supported. Use R32_FLOAT instead
            switch(NumComponents)
            {
                case 1:
                    return GL_R32UI;
                case 2:
                    return GL_RG32UI;
                case 3:
                    return GL_RGB32UI;
                case 4:
                    return GL_RGBA32UI;
                default:
                    UNEXPECTED("Unusupported number of components");
                    return 0;
            }
        }

        case VT_INT16: {
            if(bIsNormalized)
            {
                switch(NumComponents)
                {
                    case 1:
                        return GL_R16_SNORM;
                    case 2:
                        return GL_RG16_SNORM;
                    case 4:
                        return GL_RGBA16_SNORM;
                    default:
                        UNEXPECTED("Unusupported number of components");
                        return 0;
                }
            }
            else
            {
                switch(NumComponents)
                {
                    case 1:
                        return GL_R16I;
                    case 2:
                        return GL_RG16I;
                    case 4:
                        return GL_RGBA16I;
                    default:
                        UNEXPECTED("Unusupported number of components");
                        return 0;
                }
            }
        }

        case VT_UINT16: {
            if(bIsNormalized)
            {
                switch(NumComponents)
                {
                    case 1:
                        return GL_R16;
                    case 2:
                        return GL_RG16;
                    case 4:
                        return GL_RGBA16;
                    default:
                        UNEXPECTED("Unusupported number of components");
                        return 0;
                }
            }
            else
            {
                switch(NumComponents)
                {
                    case 1:
                        return GL_R16UI;
                    case 2:
                        return GL_RG16UI;
                    case 4:
                        return GL_RGBA16UI;
                    default:
                        UNEXPECTED("Unusupported number of components");
                        return 0;
                }
            }
        }

        case VT_INT8: {
            if(bIsNormalized)
            {
                switch(NumComponents)
                {
                    case 1:
                        return GL_R8_SNORM;
                    case 2:
                        return GL_RG8_SNORM;
                    case 4:
                        return GL_RGBA8_SNORM;
                    default:
                        UNEXPECTED("Unusupported number of components");
                        return 0;
                }
            }
            else
            {
                switch(NumComponents)
                {
                    case 1:
                        return GL_R8I;
                    case 2:
                        return GL_RG8I;
                    case 4:
                        return GL_RGBA8I;
                    default:
                        UNEXPECTED("Unusupported number of components");
                        return 0;
                }
            }
        }

        case VT_UINT8: {
            if(bIsNormalized)
            {
                switch(NumComponents)
                {
                    case 1:
                        return GL_R8;
                    case 2:
                        return GL_RG8;
                    case 4:
                        return GL_RGBA8;
                    default:
                        UNEXPECTED("Unusupported number of components");
                        return 0;
                }
            }
            else
            {
                switch(NumComponents)
                {
                    case 1:
                        return GL_R8UI;
                    case 2:
                        return GL_RG8UI;
                    case 4:
                        return GL_RGBA8UI;
                    default:
                        UNEXPECTED("Unusupported number of components");
                        return 0;
                }
            }
        }

        default:
            UNEXPECTED("Unusupported format");
            return 0;
    }
}

GLenum StencilOp2GlStencilOp(STENCIL_OP StencilOp)
{
    static bool   bIsInit                          = false;
    static GLenum GLStencilOps[STENCIL_OP_NUM_OPS] = {};
    if(!bIsInit)
    {
        GLStencilOps[STENCIL_OP_KEEP]      = GL_KEEP;
        GLStencilOps[STENCIL_OP_ZERO]      = GL_ZERO;
        GLStencilOps[STENCIL_OP_REPLACE]   = GL_REPLACE;
        GLStencilOps[STENCIL_OP_INCR_SAT]  = GL_INCR;
        GLStencilOps[STENCIL_OP_DECR_SAT]  = GL_DECR;
        GLStencilOps[STENCIL_OP_INVERT]    = GL_INVERT;
        GLStencilOps[STENCIL_OP_INCR_WRAP] = GL_INCR_WRAP;
        GLStencilOps[STENCIL_OP_DECR_WRAP] = GL_DECR_WRAP;
        bIsInit                            = true;
    }

    if(StencilOp > STENCIL_OP_UNDEFINED && StencilOp < STENCIL_OP_NUM_OPS)
    {
        auto GlStencilOp = GLStencilOps[StencilOp];
        assert(GlStencilOp != 0
               || GlStencilOp == 0 && StencilOp == STENCIL_OP_ZERO);   // Unexpected stencil op
        return GlStencilOp;
    }
    else
    {
        UNEXPECTED("Stencil operation (", StencilOp, ") is out of allowed range [1, ", STENCIL_OP_NUM_OPS - 1,
                   "]");
        return 0;
    }
}

GLenum BlendFactor2GLBlend(BLEND_FACTOR bf)
{
    static bool   bIsInit                           = false;
    static GLenum GLBlend[BLEND_FACTOR_NUM_FACTORS] = {};
    if(!bIsInit)
    {
        GLBlend[BLEND_FACTOR_ZERO]             = GL_ZERO;
        GLBlend[BLEND_FACTOR_ONE]              = GL_ONE;
        GLBlend[BLEND_FACTOR_SRC_COLOR]        = GL_SRC_COLOR;
        GLBlend[BLEND_FACTOR_INV_SRC_COLOR]    = GL_ONE_MINUS_SRC_COLOR;
        GLBlend[BLEND_FACTOR_SRC_ALPHA]        = GL_SRC_ALPHA;
        GLBlend[BLEND_FACTOR_INV_SRC_ALPHA]    = GL_ONE_MINUS_SRC_ALPHA;
        GLBlend[BLEND_FACTOR_DEST_ALPHA]       = GL_DST_ALPHA;
        GLBlend[BLEND_FACTOR_INV_DEST_ALPHA]   = GL_ONE_MINUS_DST_ALPHA;
        GLBlend[BLEND_FACTOR_DEST_COLOR]       = GL_DST_COLOR;
        GLBlend[BLEND_FACTOR_INV_DEST_COLOR]   = GL_ONE_MINUS_DST_COLOR;
        GLBlend[BLEND_FACTOR_SRC_ALPHA_SAT]    = GL_SRC_ALPHA_SATURATE;
        GLBlend[BLEND_FACTOR_BLEND_FACTOR]     = GL_CONSTANT_COLOR;
        GLBlend[BLEND_FACTOR_INV_BLEND_FACTOR] = GL_ONE_MINUS_CONSTANT_COLOR;
        GLBlend[BLEND_FACTOR_SRC1_COLOR]       = GL_SRC1_COLOR;
        GLBlend[BLEND_FACTOR_INV_SRC1_COLOR]   = GL_ONE_MINUS_SRC1_COLOR;
        GLBlend[BLEND_FACTOR_SRC1_ALPHA]       = GL_SRC1_ALPHA;
        GLBlend[BLEND_FACTOR_INV_SRC1_ALPHA]   = GL_ONE_MINUS_SRC1_ALPHA;

        bIsInit = true;
    }
    if(bf > BLEND_FACTOR_UNDEFINED && bf < BLEND_FACTOR_NUM_FACTORS)
    {
        auto glbf = GLBlend[bf];
        assert(glbf != 0 || glbf == 0 && bf == BLEND_FACTOR_ZERO);   // Incorrect blend factor
        return glbf;
    }
    else
    {
        UNEXPECTED("Incorrect blend factor (", bf, ")");
        return 0;
    }
}

GLenum BlendOperation2GLBlendOp(BLEND_OPERATION BlendOp)
{
    static bool   bIsInit                                   = false;
    static GLenum GLBlendOp[BLEND_OPERATION_NUM_OPERATIONS] = {};
    if(!bIsInit)
    {
        GLBlendOp[BLEND_OPERATION_ADD]          = GL_FUNC_ADD;
        GLBlendOp[BLEND_OPERATION_SUBTRACT]     = GL_FUNC_SUBTRACT;
        GLBlendOp[BLEND_OPERATION_REV_SUBTRACT] = GL_FUNC_REVERSE_SUBTRACT;
        GLBlendOp[BLEND_OPERATION_MIN]          = GL_MIN;
        GLBlendOp[BLEND_OPERATION_MAX]          = GL_MAX;

        bIsInit = true;
    }

    if(BlendOp > BLEND_OPERATION_UNDEFINED && BlendOp < BLEND_OPERATION_NUM_OPERATIONS)
    {
        auto glbop = GLBlendOp[BlendOp];
        assert(glbop != 0);   // Incorrect blend operation
        return glbop;
    }
    else
    {
        UNEXPECTED("Incorrect blend operation (", BlendOp, ")");
        return 0;
    }
}
}   // namespace evnt
