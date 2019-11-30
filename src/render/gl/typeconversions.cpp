#include "typeconversions.h"
#include "../itextureview.h"
#include "../utils/graphicsaccessories.h"
#include <unordered_map>

namespace evnt
{
class FormatToGLInternalTexFormatMap
{
public:
    FormatToGLInternalTexFormatMap()
    {
        // http://www.opengl.org/wiki/Image_Format
        m_fmt_to_glfmt_map[TEX_FORMAT_UNKNOWN] = 0;

        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA32_TYPELESS] = GL_RGBA32F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA32_FLOAT]    = GL_RGBA32F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA32_UINT]     = GL_RGBA32UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA32_SINT]     = GL_RGBA32I;

        m_fmt_to_glfmt_map[TEX_FORMAT_RGB32_TYPELESS] = GL_RGB32F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGB32_FLOAT]    = GL_RGB32F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGB32_UINT]     = GL_RGB32UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGB32_SINT]     = GL_RGB32I;

        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA16_TYPELESS] = GL_RGBA16F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA16_FLOAT]    = GL_RGBA16F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA16_UNORM]    = GL_RGBA16;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA16_UINT]     = GL_RGBA16UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA16_SNORM]    = GL_RGBA16_SNORM;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA16_SINT]     = GL_RGBA16I;

        m_fmt_to_glfmt_map[TEX_FORMAT_RG32_TYPELESS] = GL_RG32F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG32_FLOAT]    = GL_RG32F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG32_UINT]     = GL_RG32UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG32_SINT]     = GL_RG32I;

        m_fmt_to_glfmt_map[TEX_FORMAT_R32G8X24_TYPELESS]        = GL_DEPTH32F_STENCIL8;
        m_fmt_to_glfmt_map[TEX_FORMAT_D32_FLOAT_S8X24_UINT]     = GL_DEPTH32F_STENCIL8;
        m_fmt_to_glfmt_map[TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS] = GL_DEPTH32F_STENCIL8;
        m_fmt_to_glfmt_map[TEX_FORMAT_X32_TYPELESS_G8X24_UINT]  = 0;   // GL_DEPTH32F_STENCIL8;

        m_fmt_to_glfmt_map[TEX_FORMAT_RGB10A2_TYPELESS] = GL_RGB10_A2;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGB10A2_UNORM]    = GL_RGB10_A2;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGB10A2_UINT]     = GL_RGB10_A2UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_R11G11B10_FLOAT]  = GL_R11F_G11F_B10F;

        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA8_TYPELESS]   = GL_RGBA8;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA8_UNORM]      = GL_RGBA8;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA8_UNORM_SRGB] = GL_SRGB8_ALPHA8;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA8_UINT]       = GL_RGBA8UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA8_SNORM]      = GL_RGBA8_SNORM;
        m_fmt_to_glfmt_map[TEX_FORMAT_RGBA8_SINT]       = GL_RGBA8I;

        m_fmt_to_glfmt_map[TEX_FORMAT_RG16_TYPELESS] = GL_RG16F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG16_FLOAT]    = GL_RG16F;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG16_UNORM]    = GL_RG16;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG16_UINT]     = GL_RG16UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG16_SNORM]    = GL_RG16_SNORM;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG16_SINT]     = GL_RG16I;

        m_fmt_to_glfmt_map[TEX_FORMAT_R32_TYPELESS] = GL_R32F;
        m_fmt_to_glfmt_map[TEX_FORMAT_D32_FLOAT]    = GL_DEPTH_COMPONENT32F;
        m_fmt_to_glfmt_map[TEX_FORMAT_R32_FLOAT]    = GL_R32F;
        m_fmt_to_glfmt_map[TEX_FORMAT_R32_UINT]     = GL_R32UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_R32_SINT]     = GL_R32I;

        m_fmt_to_glfmt_map[TEX_FORMAT_R24G8_TYPELESS]        = GL_DEPTH24_STENCIL8;
        m_fmt_to_glfmt_map[TEX_FORMAT_D24_UNORM_S8_UINT]     = GL_DEPTH24_STENCIL8;
        m_fmt_to_glfmt_map[TEX_FORMAT_R24_UNORM_X8_TYPELESS] = GL_DEPTH24_STENCIL8;
        m_fmt_to_glfmt_map[TEX_FORMAT_X24_TYPELESS_G8_UINT]  = 0;   // GL_DEPTH24_STENCIL8;

        m_fmt_to_glfmt_map[TEX_FORMAT_RG8_TYPELESS] = GL_RG8;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG8_UNORM]    = GL_RG8;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG8_UINT]     = GL_RG8UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG8_SNORM]    = GL_RG8_SNORM;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG8_SINT]     = GL_RG8I;

        m_fmt_to_glfmt_map[TEX_FORMAT_R16_TYPELESS] = GL_R16F;
        m_fmt_to_glfmt_map[TEX_FORMAT_R16_FLOAT]    = GL_R16F;
        m_fmt_to_glfmt_map[TEX_FORMAT_D16_UNORM]    = GL_DEPTH_COMPONENT16;
        m_fmt_to_glfmt_map[TEX_FORMAT_R16_UNORM]    = GL_R16;
        m_fmt_to_glfmt_map[TEX_FORMAT_R16_UINT]     = GL_R16UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_R16_SNORM]    = GL_R16_SNORM;
        m_fmt_to_glfmt_map[TEX_FORMAT_R16_SINT]     = GL_R16I;

        m_fmt_to_glfmt_map[TEX_FORMAT_R8_TYPELESS] = GL_R8;
        m_fmt_to_glfmt_map[TEX_FORMAT_R8_UNORM]    = GL_R8;
        m_fmt_to_glfmt_map[TEX_FORMAT_R8_UINT]     = GL_R8UI;
        m_fmt_to_glfmt_map[TEX_FORMAT_R8_SNORM]    = GL_R8_SNORM;
        m_fmt_to_glfmt_map[TEX_FORMAT_R8_SINT]     = GL_R8I;
        m_fmt_to_glfmt_map[TEX_FORMAT_A8_UNORM]    = 0;

        m_fmt_to_glfmt_map[TEX_FORMAT_R1_UNORM] = 0;

        m_fmt_to_glfmt_map[TEX_FORMAT_RGB9E5_SHAREDEXP] = GL_RGB9_E5;
        m_fmt_to_glfmt_map[TEX_FORMAT_RG8_B8G8_UNORM]   = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_G8R8_G8B8_UNORM]  = 0;

        // http://www.g-truc.net/post-0335.html
        // http://renderingpipeline.com/2012/07/texture-compression/
        m_fmt_to_glfmt_map[TEX_FORMAT_BC1_TYPELESS] = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC1_UNORM] =
            GL_COMPRESSED_RGB_S3TC_DXT1_EXT;   // GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC1_UNORM_SRGB] =
            GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;   // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC2_TYPELESS]               = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC2_UNORM]                  = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC2_UNORM_SRGB]             = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC3_TYPELESS]               = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC3_UNORM]                  = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC3_UNORM_SRGB]             = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC4_TYPELESS]               = GL_COMPRESSED_RED_RGTC1;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC4_UNORM]                  = GL_COMPRESSED_RED_RGTC1;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC4_SNORM]                  = GL_COMPRESSED_SIGNED_RED_RGTC1;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC5_TYPELESS]               = GL_COMPRESSED_RG_RGTC2;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC5_UNORM]                  = GL_COMPRESSED_RG_RGTC2;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC5_SNORM]                  = GL_COMPRESSED_SIGNED_RG_RGTC2;
        m_fmt_to_glfmt_map[TEX_FORMAT_B5G6R5_UNORM]               = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_B5G5R5A1_UNORM]             = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_BGRA8_UNORM]                = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_BGRX8_UNORM]                = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM] = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_BGRA8_TYPELESS]             = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_BGRA8_UNORM_SRGB]           = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_BGRX8_TYPELESS]             = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_BGRX8_UNORM_SRGB]           = 0;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC6H_TYPELESS]              = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC6H_UF16]                  = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC6H_SF16]                  = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC7_TYPELESS]               = GL_COMPRESSED_RGBA_BPTC_UNORM;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC7_UNORM]                  = GL_COMPRESSED_RGBA_BPTC_UNORM;
        m_fmt_to_glfmt_map[TEX_FORMAT_BC7_UNORM_SRGB]             = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
    }

    GLenum operator[](TEXTURE_FORMAT tex_format) const
    {
        assert(tex_format < std::size(m_fmt_to_glfmt_map));
        return m_fmt_to_glfmt_map[tex_format];
    }

private:
    GLenum m_fmt_to_glfmt_map[TEX_FORMAT_NUM_FORMATS] = {0};
};

GLenum TexFormatToGLInternalTexFormat(TEXTURE_FORMAT tex_format, uint32_t bind_flags)
{
    static const FormatToGLInternalTexFormatMap format_map;
    if(tex_format >= TEX_FORMAT_UNKNOWN && tex_format < TEX_FORMAT_NUM_FORMATS)
    {
        auto gl_format = format_map[tex_format];
        if(bind_flags != 0)
            gl_format = CorrectGLTexFormat(gl_format, bind_flags);
        return gl_format;
    }
    else
    {
        UNEXPECTED("Texture format (", int{tex_format}, ") out of allowed range [0, ",
                   int{TEX_FORMAT_NUM_FORMATS} - 1, "]");
        return 0;
    }
}

class InternalTexFormatToTexFormatMap
{
public:
    InternalTexFormatToTexFormatMap()
    {
        for(TEXTURE_FORMAT tex_fmt = TEX_FORMAT_UNKNOWN; tex_fmt < TEX_FORMAT_NUM_FORMATS;
            tex_fmt                = static_cast<TEXTURE_FORMAT>(static_cast<int>(tex_fmt) + 1))
        {
            auto component_type = GetTextureFormatInfo(tex_fmt).component_type;
            if(component_type == COMPONENT_TYPE_UNDEFINED || component_type == COMPONENT_TYPE_DEPTH_STENCIL
               || tex_fmt == TEX_FORMAT_RGB10A2_TYPELESS || tex_fmt == TEX_FORMAT_BC1_TYPELESS
               || tex_fmt == TEX_FORMAT_BC2_TYPELESS || tex_fmt == TEX_FORMAT_BC3_TYPELESS
               || tex_fmt == TEX_FORMAT_BC4_TYPELESS || tex_fmt == TEX_FORMAT_BC5_TYPELESS
               || tex_fmt == TEX_FORMAT_BC6H_TYPELESS || tex_fmt == TEX_FORMAT_BC7_TYPELESS)
                continue;   // Skip typeless and depth-stencil formats
            auto gl_tex_format = TexFormatToGLInternalTexFormat(tex_fmt);
            if(gl_tex_format != 0)
            {
                assert(m_format_map.find(gl_tex_format) == m_format_map.end());
                m_format_map[gl_tex_format] = tex_fmt;
            }
            m_format_map[TexFormatToGLInternalTexFormat(TEX_FORMAT_D32_FLOAT_S8X24_UINT)] =
                TEX_FORMAT_D32_FLOAT_S8X24_UINT;
            m_format_map[TexFormatToGLInternalTexFormat(TEX_FORMAT_D24_UNORM_S8_UINT)] =
                TEX_FORMAT_D24_UNORM_S8_UINT;
        }
    }

    TEXTURE_FORMAT operator[](GLenum gl_format) const
    {
        auto format_it = m_format_map.find(gl_format);
        if(format_it != m_format_map.end())
        {
            assert(gl_format == TexFormatToGLInternalTexFormat(format_it->second));
            return format_it->second;
        }
        else
        {
            UNEXPECTED("Unknown GL format");
            return TEX_FORMAT_UNKNOWN;
        }
    }

private:
    std::unordered_map<GLenum, TEXTURE_FORMAT> m_format_map;
};

TEXTURE_FORMAT GLInternalTexFormatToTexFormat(GLenum gl_format)
{
    static const InternalTexFormatToTexFormatMap format_map;
    return format_map[gl_format];
}

GLenum CorrectGLTexFormat(GLenum gl_tex_format, uint32_t bind_flags)
{
    if(bind_flags & BIND_DEPTH_STENCIL)
    {
        if(gl_tex_format == GL_R32F)
            gl_tex_format = GL_DEPTH_COMPONENT32F;
        if(gl_tex_format == GL_R16)
            gl_tex_format = GL_DEPTH_COMPONENT16;
    }
    return gl_tex_format;
}

NativePixelAttribs GetNativePixelTransferAttribs(TEXTURE_FORMAT tex_format)
{
    // http://www.opengl.org/wiki/Pixel_Transfer

    static bool               attribs_map_intialized = false;
    static NativePixelAttribs fmt_to_glpixelfmt[TEX_FORMAT_NUM_FORMATS];
    if(!attribs_map_intialized)
    {
        // http://www.opengl.org/wiki/Image_Format
        fmt_to_glpixelfmt[TEX_FORMAT_UNKNOWN] = NativePixelAttribs();

        fmt_to_glpixelfmt[TEX_FORMAT_RGBA32_TYPELESS] = NativePixelAttribs(GL_RGBA, GL_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA32_FLOAT]    = NativePixelAttribs(GL_RGBA, GL_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA32_UINT]     = NativePixelAttribs(GL_RGBA_INTEGER, GL_UNSIGNED_INT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA32_SINT]     = NativePixelAttribs(GL_RGBA_INTEGER, GL_INT);

        fmt_to_glpixelfmt[TEX_FORMAT_RGB32_TYPELESS] = NativePixelAttribs(GL_RGB, GL_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGB32_FLOAT]    = NativePixelAttribs(GL_RGB, GL_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGB32_UINT]     = NativePixelAttribs(GL_RGB_INTEGER, GL_UNSIGNED_INT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGB32_SINT]     = NativePixelAttribs(GL_RGB_INTEGER, GL_INT);

        fmt_to_glpixelfmt[TEX_FORMAT_RGBA16_TYPELESS] = NativePixelAttribs(GL_RGBA, GL_HALF_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA16_FLOAT]    = NativePixelAttribs(GL_RGBA, GL_HALF_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA16_UNORM]    = NativePixelAttribs(GL_RGBA, GL_UNSIGNED_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA16_UINT]  = NativePixelAttribs(GL_RGBA_INTEGER, GL_UNSIGNED_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA16_SNORM] = NativePixelAttribs(GL_RGBA, GL_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA16_SINT]  = NativePixelAttribs(GL_RGBA_INTEGER, GL_SHORT);

        fmt_to_glpixelfmt[TEX_FORMAT_RG32_TYPELESS] = NativePixelAttribs(GL_RG, GL_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RG32_FLOAT]    = NativePixelAttribs(GL_RG, GL_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RG32_UINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_UNSIGNED_INT);
        fmt_to_glpixelfmt[TEX_FORMAT_RG32_SINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_INT);

        fmt_to_glpixelfmt[TEX_FORMAT_R32G8X24_TYPELESS] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
        fmt_to_glpixelfmt[TEX_FORMAT_D32_FLOAT_S8X24_UINT] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
        fmt_to_glpixelfmt[TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
        fmt_to_glpixelfmt[TEX_FORMAT_X32_TYPELESS_G8X24_UINT] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);

        // Components are normally packed with the first component in the most significant
        // bits of the bitfield, and successive component occupying progressively less
        // significant locations.Types whose token names end with _REV reverse the component
        // packing order from least to most significant locations.
        fmt_to_glpixelfmt[TEX_FORMAT_RGB10A2_TYPELESS] =
            NativePixelAttribs(GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV);
        fmt_to_glpixelfmt[TEX_FORMAT_RGB10A2_UNORM] =
            NativePixelAttribs(GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV);
        fmt_to_glpixelfmt[TEX_FORMAT_RGB10A2_UINT] =
            NativePixelAttribs(GL_RGBA_INTEGER, GL_UNSIGNED_INT_2_10_10_10_REV);
        fmt_to_glpixelfmt[TEX_FORMAT_R11G11B10_FLOAT] =
            NativePixelAttribs(GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV);

        fmt_to_glpixelfmt[TEX_FORMAT_RGBA8_TYPELESS]   = NativePixelAttribs(GL_RGBA, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA8_UNORM]      = NativePixelAttribs(GL_RGBA, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA8_UNORM_SRGB] = NativePixelAttribs(GL_RGBA, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA8_UINT]  = NativePixelAttribs(GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA8_SNORM] = NativePixelAttribs(GL_RGBA, GL_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_RGBA8_SINT]  = NativePixelAttribs(GL_RGBA_INTEGER, GL_BYTE);

        fmt_to_glpixelfmt[TEX_FORMAT_RG16_TYPELESS] = NativePixelAttribs(GL_RG, GL_HALF_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RG16_FLOAT]    = NativePixelAttribs(GL_RG, GL_HALF_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_RG16_UNORM]    = NativePixelAttribs(GL_RG, GL_UNSIGNED_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_RG16_UINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_UNSIGNED_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_RG16_SNORM]    = NativePixelAttribs(GL_RG, GL_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_RG16_SINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_SHORT);

        fmt_to_glpixelfmt[TEX_FORMAT_R32_TYPELESS] = NativePixelAttribs(GL_RED, GL_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_D32_FLOAT]    = NativePixelAttribs(GL_DEPTH_COMPONENT, GL_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_R32_FLOAT]    = NativePixelAttribs(GL_RED, GL_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_R32_UINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_UNSIGNED_INT);
        fmt_to_glpixelfmt[TEX_FORMAT_R32_SINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_INT);

        fmt_to_glpixelfmt[TEX_FORMAT_R24G8_TYPELESS] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
        fmt_to_glpixelfmt[TEX_FORMAT_D24_UNORM_S8_UINT] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
        fmt_to_glpixelfmt[TEX_FORMAT_R24_UNORM_X8_TYPELESS] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
        fmt_to_glpixelfmt[TEX_FORMAT_X24_TYPELESS_G8_UINT] =
            NativePixelAttribs(GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);

        fmt_to_glpixelfmt[TEX_FORMAT_RG8_TYPELESS] = NativePixelAttribs(GL_RG, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_RG8_UNORM]    = NativePixelAttribs(GL_RG, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_RG8_UINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_RG8_SNORM]    = NativePixelAttribs(GL_RG, GL_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_RG8_SINT]     = NativePixelAttribs(GL_RG_INTEGER, GL_BYTE);

        fmt_to_glpixelfmt[TEX_FORMAT_R16_TYPELESS] = NativePixelAttribs(GL_RED, GL_HALF_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_R16_FLOAT]    = NativePixelAttribs(GL_RED, GL_HALF_FLOAT);
        fmt_to_glpixelfmt[TEX_FORMAT_D16_UNORM] = NativePixelAttribs(GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_R16_UNORM] = NativePixelAttribs(GL_RED, GL_UNSIGNED_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_R16_UINT]  = NativePixelAttribs(GL_RED_INTEGER, GL_UNSIGNED_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_R16_SNORM] = NativePixelAttribs(GL_RED, GL_SHORT);
        fmt_to_glpixelfmt[TEX_FORMAT_R16_SINT]  = NativePixelAttribs(GL_RED_INTEGER, GL_SHORT);

        fmt_to_glpixelfmt[TEX_FORMAT_R8_TYPELESS] = NativePixelAttribs(GL_RED, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_R8_UNORM]    = NativePixelAttribs(GL_RED, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_R8_UINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_UNSIGNED_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_R8_SNORM]    = NativePixelAttribs(GL_RED, GL_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_R8_SINT]     = NativePixelAttribs(GL_RED_INTEGER, GL_BYTE);
        fmt_to_glpixelfmt[TEX_FORMAT_A8_UNORM]    = NativePixelAttribs();

        fmt_to_glpixelfmt[TEX_FORMAT_R1_UNORM] = NativePixelAttribs();

        fmt_to_glpixelfmt[TEX_FORMAT_RGB9E5_SHAREDEXP] =
            NativePixelAttribs(GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV);
        fmt_to_glpixelfmt[TEX_FORMAT_RG8_B8G8_UNORM]  = NativePixelAttribs();
        fmt_to_glpixelfmt[TEX_FORMAT_G8R8_G8B8_UNORM] = NativePixelAttribs();

        // http://www.g-truc.net/post-0335.html
        // http://renderingpipeline.com/2012/07/texture-compression/
        fmt_to_glpixelfmt[TEX_FORMAT_BC1_TYPELESS]   = NativePixelAttribs(GL_RGB, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC1_UNORM]      = NativePixelAttribs(GL_RGB, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC1_UNORM_SRGB] = NativePixelAttribs(GL_RGB, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC2_TYPELESS]   = NativePixelAttribs(GL_RGBA, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC2_UNORM]      = NativePixelAttribs(GL_RGBA, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC2_UNORM_SRGB] = NativePixelAttribs(GL_RGBA, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC3_TYPELESS]   = NativePixelAttribs(GL_RGBA, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC3_UNORM]      = NativePixelAttribs(GL_RGBA, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC3_UNORM_SRGB] = NativePixelAttribs(GL_RGBA, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC4_TYPELESS]   = NativePixelAttribs(GL_RED, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC4_UNORM]      = NativePixelAttribs(GL_RED, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC4_SNORM]      = NativePixelAttribs(GL_RED, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC5_TYPELESS]   = NativePixelAttribs(GL_RG, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC5_UNORM]      = NativePixelAttribs(GL_RG, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC5_SNORM]      = NativePixelAttribs(GL_RG, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_B5G6R5_UNORM] = NativePixelAttribs(GL_RGB, GL_UNSIGNED_SHORT_5_6_5_REV);
        fmt_to_glpixelfmt[TEX_FORMAT_B5G5R5A1_UNORM] =
            NativePixelAttribs(GL_RGB, GL_UNSIGNED_SHORT_1_5_5_5_REV);
        fmt_to_glpixelfmt[TEX_FORMAT_BGRA8_UNORM]                = NativePixelAttribs();
        fmt_to_glpixelfmt[TEX_FORMAT_BGRX8_UNORM]                = NativePixelAttribs();
        fmt_to_glpixelfmt[TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM] = NativePixelAttribs();
        fmt_to_glpixelfmt[TEX_FORMAT_BGRA8_TYPELESS]             = NativePixelAttribs();
        fmt_to_glpixelfmt[TEX_FORMAT_BGRA8_UNORM_SRGB]           = NativePixelAttribs();
        fmt_to_glpixelfmt[TEX_FORMAT_BGRX8_TYPELESS]             = NativePixelAttribs();
        fmt_to_glpixelfmt[TEX_FORMAT_BGRX8_UNORM_SRGB]           = NativePixelAttribs();
        fmt_to_glpixelfmt[TEX_FORMAT_BC6H_TYPELESS]              = NativePixelAttribs(GL_RGB, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC6H_UF16]                  = NativePixelAttribs(GL_RGB, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC6H_SF16]                  = NativePixelAttribs(GL_RGB, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC7_TYPELESS]               = NativePixelAttribs(GL_RGBA, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC7_UNORM]                  = NativePixelAttribs(GL_RGBA, 0, true);
        fmt_to_glpixelfmt[TEX_FORMAT_BC7_UNORM_SRGB]             = NativePixelAttribs(GL_RGBA, 0, true);

        attribs_map_intialized = true;
    }

    if(tex_format > TEX_FORMAT_UNKNOWN && tex_format < TEX_FORMAT_NUM_FORMATS)
    {
        return fmt_to_glpixelfmt[tex_format];
    }
    else
    {
        UNEXPECTED("Texture format (", int{tex_format}, ") is out of allowed range [1, ",
                   int{TEX_FORMAT_NUM_FORMATS} - 1, "]");
        return fmt_to_glpixelfmt[0];
    }
}

GLenum AccessFlags2GLAccess(uint32_t uav_access_flags)
{
    static GLenum accessflags_to_glaccess_map[UAV_ACCESS_FLAG_READ_WRITE + 1] = {};
    static bool   is_init                                                     = false;
    if(!is_init)
    {
        accessflags_to_glaccess_map[UAV_ACCESS_FLAG_READ]       = GL_READ_ONLY;
        accessflags_to_glaccess_map[UAV_ACCESS_FLAG_WRITE]      = GL_WRITE_ONLY;
        accessflags_to_glaccess_map[UAV_ACCESS_FLAG_READ_WRITE] = GL_READ_WRITE;
        is_init                                                 = true;
    }

    auto glaccess = accessflags_to_glaccess_map[uav_access_flags & UAV_ACCESS_FLAG_READ_WRITE];
    return glaccess;
}

GLenum TypeToGLTexFormat(VALUE_TYPE val_type, uint32_t num_components, bool is_normalized)
{
    switch(val_type)
    {
        case VT_FLOAT16: {
            assert(!is_normalized);   // Floating point formats cannot be normalized
            switch(num_components)
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
            assert(!is_normalized);   // Floating point formats cannot be normalized
            switch(num_components)
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
            assert(!is_normalized);   // 32-bit UNORM formats are not supported. Use R32_FLOAT instead
            switch(num_components)
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
            assert(!is_normalized);   // 32-bit UNORM formats are not supported. Use R32_FLOAT instead
            switch(num_components)
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
            if(is_normalized)
            {
                switch(num_components)
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
                switch(num_components)
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
            if(is_normalized)
            {
                switch(num_components)
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
                switch(num_components)
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
            if(is_normalized)
            {
                switch(num_components)
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
                switch(num_components)
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
            if(is_normalized)
            {
                switch(num_components)
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
                switch(num_components)
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

GLenum StencilOp2GlStencilOp(STENCIL_OP stencil_op)
{
    static bool   is_init                           = false;
    static GLenum glstencil_ops[STENCIL_OP_NUM_OPS] = {};
    if(!is_init)
    {
        glstencil_ops[STENCIL_OP_KEEP]      = GL_KEEP;
        glstencil_ops[STENCIL_OP_ZERO]      = GL_ZERO;
        glstencil_ops[STENCIL_OP_REPLACE]   = GL_REPLACE;
        glstencil_ops[STENCIL_OP_INCR_SAT]  = GL_INCR;
        glstencil_ops[STENCIL_OP_DECR_SAT]  = GL_DECR;
        glstencil_ops[STENCIL_OP_INVERT]    = GL_INVERT;
        glstencil_ops[STENCIL_OP_INCR_WRAP] = GL_INCR_WRAP;
        glstencil_ops[STENCIL_OP_DECR_WRAP] = GL_DECR_WRAP;
        is_init                             = true;
    }

    if(stencil_op > STENCIL_OP_UNDEFINED && stencil_op < STENCIL_OP_NUM_OPS)
    {
        auto glstencil_op = glstencil_ops[stencil_op];
        assert(glstencil_op != 0
               || (glstencil_op == 0 && stencil_op == STENCIL_OP_ZERO));   // Unexpected stencil op
        return glstencil_op;
    }
    else
    {
        UNEXPECTED("Stencil operation (", stencil_op, ") is out of allowed range [1, ",
                   STENCIL_OP_NUM_OPS - 1, "]");
        return 0;
    }
}

GLenum BlendFactor2GLBlend(BLEND_FACTOR bf)
{
    static bool   is_init                           = false;
    static GLenum glblend[BLEND_FACTOR_NUM_FACTORS] = {};
    if(!is_init)
    {
        glblend[BLEND_FACTOR_ZERO]             = GL_ZERO;
        glblend[BLEND_FACTOR_ONE]              = GL_ONE;
        glblend[BLEND_FACTOR_SRC_COLOR]        = GL_SRC_COLOR;
        glblend[BLEND_FACTOR_INV_SRC_COLOR]    = GL_ONE_MINUS_SRC_COLOR;
        glblend[BLEND_FACTOR_SRC_ALPHA]        = GL_SRC_ALPHA;
        glblend[BLEND_FACTOR_INV_SRC_ALPHA]    = GL_ONE_MINUS_SRC_ALPHA;
        glblend[BLEND_FACTOR_DEST_ALPHA]       = GL_DST_ALPHA;
        glblend[BLEND_FACTOR_INV_DEST_ALPHA]   = GL_ONE_MINUS_DST_ALPHA;
        glblend[BLEND_FACTOR_DEST_COLOR]       = GL_DST_COLOR;
        glblend[BLEND_FACTOR_INV_DEST_COLOR]   = GL_ONE_MINUS_DST_COLOR;
        glblend[BLEND_FACTOR_SRC_ALPHA_SAT]    = GL_SRC_ALPHA_SATURATE;
        glblend[BLEND_FACTOR_BLEND_FACTOR]     = GL_CONSTANT_COLOR;
        glblend[BLEND_FACTOR_INV_BLEND_FACTOR] = GL_ONE_MINUS_CONSTANT_COLOR;
        glblend[BLEND_FACTOR_SRC1_COLOR]       = GL_SRC1_COLOR;
        glblend[BLEND_FACTOR_INV_SRC1_COLOR]   = GL_ONE_MINUS_SRC1_COLOR;
        glblend[BLEND_FACTOR_SRC1_ALPHA]       = GL_SRC1_ALPHA;
        glblend[BLEND_FACTOR_INV_SRC1_ALPHA]   = GL_ONE_MINUS_SRC1_ALPHA;

        is_init = true;
    }
    if(bf > BLEND_FACTOR_UNDEFINED && bf < BLEND_FACTOR_NUM_FACTORS)
    {
        auto glbf = glblend[bf];
        assert(glbf != 0 || (glbf == 0 && bf == BLEND_FACTOR_ZERO));   // Incorrect blend factor
        return glbf;
    }
    else
    {
        UNEXPECTED("Incorrect blend factor (", bf, ")");
        return 0;
    }
}

GLenum BlendOperation2GLBlendOp(BLEND_OPERATION blend_op)
{
    static bool   is_init                                    = false;
    static GLenum glblend_op[BLEND_OPERATION_NUM_OPERATIONS] = {};
    if(!is_init)
    {
        glblend_op[BLEND_OPERATION_ADD]          = GL_FUNC_ADD;
        glblend_op[BLEND_OPERATION_SUBTRACT]     = GL_FUNC_SUBTRACT;
        glblend_op[BLEND_OPERATION_REV_SUBTRACT] = GL_FUNC_REVERSE_SUBTRACT;
        glblend_op[BLEND_OPERATION_MIN]          = GL_MIN;
        glblend_op[BLEND_OPERATION_MAX]          = GL_MAX;

        is_init = true;
    }

    if(blend_op > BLEND_OPERATION_UNDEFINED && blend_op < BLEND_OPERATION_NUM_OPERATIONS)
    {
        auto glbop = glblend_op[blend_op];
        assert(glbop != 0);   // Incorrect blend operation
        return glbop;
    }
    else
    {
        UNEXPECTED("Incorrect blend operation (", blend_op, ")");
        return 0;
    }
}
}   // namespace evnt
