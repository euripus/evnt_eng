#include "graphicsaccessories.h"
#include "../../log/debug_messages.h"
#include <algorithm>
#include <cassert>

namespace evnt
{
const char * GetValueTypeString(VALUE_TYPE val)
{
    static const char * value_type_strings[VT_NUM_TYPES] = {};
    static bool         is_init                          = false;
    if(!is_init)
    {
#define INIT_VALUE_TYPE_STR(ValType) value_type_strings[ValType] = #ValType
        INIT_VALUE_TYPE_STR(VT_UNDEFINED);
        INIT_VALUE_TYPE_STR(VT_INT8);
        INIT_VALUE_TYPE_STR(VT_INT16);
        INIT_VALUE_TYPE_STR(VT_INT32);
        INIT_VALUE_TYPE_STR(VT_UINT8);
        INIT_VALUE_TYPE_STR(VT_UINT16);
        INIT_VALUE_TYPE_STR(VT_UINT16);
        INIT_VALUE_TYPE_STR(VT_FLOAT16);
        INIT_VALUE_TYPE_STR(VT_FLOAT32);
#undef INIT_VALUE_TYPE_STR
        static_assert(VT_NUM_TYPES == VT_FLOAT32 + 1, "Not all value type strings initialized.");
        is_init = true;
    }

    if(val >= VT_UNDEFINED && val < VT_NUM_TYPES)
    {
        return value_type_strings[val];
    }
    else
    {
        UNEXPECTED("Incorrect value type (", val, ")");
        return "unknown value type";
    }
}

class TexFormatToViewFormatConverter
{
public:
    TexFormatToViewFormatConverter()
    {
        static_assert(TEXTURE_VIEW_SHADER_RESOURCE == 1, "TEXTURE_VIEW_SHADER_RESOURCE == 1 expected");
        static_assert(TEXTURE_VIEW_RENDER_TARGET == 2, "TEXTURE_VIEW_RENDER_TARGET == 2 expected");
        static_assert(TEXTURE_VIEW_DEPTH_STENCIL == 3, "TEXTURE_VIEW_DEPTH_STENCIL == 3 expected");
        static_assert(TEXTURE_VIEW_UNORDERED_ACCESS == 4, "TEXTURE_VIEW_UNORDERED_ACCESS == 4 expected");
#define INIT_TEX_VIEW_FORMAT_INFO(tex_fmt, SRV_fmt, RTV_fmt, DSV_fmt, UAV_fmt)             \
    {                                                                                      \
        m_view_formats[tex_fmt][TEXTURE_VIEW_SHADER_RESOURCE - 1]  = TEX_FORMAT_##SRV_fmt; \
        m_view_formats[tex_fmt][TEXTURE_VIEW_RENDER_TARGET - 1]    = TEX_FORMAT_##RTV_fmt; \
        m_view_formats[tex_fmt][TEXTURE_VIEW_DEPTH_STENCIL - 1]    = TEX_FORMAT_##DSV_fmt; \
        m_view_formats[tex_fmt][TEXTURE_VIEW_UNORDERED_ACCESS - 1] = TEX_FORMAT_##UAV_fmt; \
    }

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA32_TYPELESS, RGBA32_FLOAT, RGBA32_FLOAT, UNKNOWN,
                                  RGBA32_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA32_FLOAT, RGBA32_FLOAT, RGBA32_FLOAT, UNKNOWN, RGBA32_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA32_UINT, RGBA32_UINT, RGBA32_UINT, UNKNOWN, RGBA32_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA32_SINT, RGBA32_SINT, RGBA32_SINT, UNKNOWN, RGBA32_SINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGB32_TYPELESS, RGB32_FLOAT, RGB32_FLOAT, UNKNOWN, RGB32_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGB32_FLOAT, RGB32_FLOAT, RGB32_FLOAT, UNKNOWN, RGB32_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGB32_UINT, RGB32_UINT, RGB32_UINT, UNKNOWN, RGB32_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGB32_SINT, RGB32_SINT, RGB32_SINT, UNKNOWN, RGB32_SINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA16_TYPELESS, RGBA16_FLOAT, RGBA16_FLOAT, UNKNOWN,
                                  RGBA16_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA16_FLOAT, RGBA16_FLOAT, RGBA16_FLOAT, UNKNOWN, RGBA16_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA16_UNORM, RGBA16_UNORM, RGBA16_UNORM, UNKNOWN, RGBA16_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA16_UINT, RGBA16_UINT, RGBA16_UINT, UNKNOWN, RGBA16_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA16_SNORM, RGBA16_SNORM, RGBA16_SNORM, UNKNOWN, RGBA16_SNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA16_SINT, RGBA16_SINT, RGBA16_SINT, UNKNOWN, RGBA16_SINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG32_TYPELESS, RG32_FLOAT, RG32_FLOAT, UNKNOWN, RG32_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG32_FLOAT, RG32_FLOAT, RG32_FLOAT, UNKNOWN, RG32_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG32_UINT, RG32_UINT, RG32_UINT, UNKNOWN, RG32_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG32_SINT, RG32_SINT, RG32_SINT, UNKNOWN, RG32_SINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R32G8X24_TYPELESS, R32_FLOAT_X8X24_TYPELESS, UNKNOWN,
                                  D32_FLOAT_S8X24_UINT, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_D32_FLOAT_S8X24_UINT, R32_FLOAT_X8X24_TYPELESS, UNKNOWN,
                                  D32_FLOAT_S8X24_UINT, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS, R32_FLOAT_X8X24_TYPELESS, UNKNOWN,
                                  D32_FLOAT_S8X24_UINT, R32_FLOAT_X8X24_TYPELESS);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_X32_TYPELESS_G8X24_UINT, X32_TYPELESS_G8X24_UINT, UNKNOWN,
                                  D32_FLOAT_S8X24_UINT, X32_TYPELESS_G8X24_UINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGB10A2_TYPELESS, RGB10A2_UNORM, RGB10A2_UNORM, UNKNOWN,
                                  RGB10A2_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGB10A2_UNORM, RGB10A2_UNORM, RGB10A2_UNORM, UNKNOWN,
                                  RGB10A2_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGB10A2_UINT, RGB10A2_UINT, RGB10A2_UINT, UNKNOWN, RGB10A2_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R11G11B10_FLOAT, R11G11B10_FLOAT, R11G11B10_FLOAT, UNKNOWN,
                                  R11G11B10_FLOAT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA8_TYPELESS, RGBA8_UNORM_SRGB, RGBA8_UNORM_SRGB, UNKNOWN,
                                  RGBA8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA8_UNORM, RGBA8_UNORM, RGBA8_UNORM, UNKNOWN, RGBA8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA8_UNORM_SRGB, RGBA8_UNORM_SRGB, RGBA8_UNORM_SRGB, UNKNOWN,
                                  RGBA8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA8_UINT, RGBA8_UINT, RGBA8_UINT, UNKNOWN, RGBA8_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA8_SNORM, RGBA8_SNORM, RGBA8_SNORM, UNKNOWN, RGBA8_SNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGBA8_SINT, RGBA8_SINT, RGBA8_SINT, UNKNOWN, RGBA8_SINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG16_TYPELESS, RG16_FLOAT, RG16_FLOAT, UNKNOWN, RG16_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG16_FLOAT, RG16_FLOAT, RG16_FLOAT, UNKNOWN, RG16_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG16_UNORM, RG16_UNORM, RG16_UNORM, UNKNOWN, RG16_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG16_UINT, RG16_UINT, RG16_UINT, UNKNOWN, RG16_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG16_SNORM, RG16_SNORM, RG16_SNORM, UNKNOWN, RG16_SNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG16_SINT, RG16_SINT, RG16_SINT, UNKNOWN, RG16_SINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R32_TYPELESS, R32_FLOAT, R32_FLOAT, D32_FLOAT, R32_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_D32_FLOAT, R32_FLOAT, R32_FLOAT, D32_FLOAT, R32_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R32_FLOAT, R32_FLOAT, R32_FLOAT, D32_FLOAT, R32_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R32_UINT, R32_UINT, R32_UINT, UNKNOWN, R32_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R32_SINT, R32_SINT, R32_SINT, UNKNOWN, R32_SINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R24G8_TYPELESS, R24_UNORM_X8_TYPELESS, UNKNOWN,
                                  D24_UNORM_S8_UINT, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_D24_UNORM_S8_UINT, R24_UNORM_X8_TYPELESS, UNKNOWN,
                                  D24_UNORM_S8_UINT, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R24_UNORM_X8_TYPELESS, R24_UNORM_X8_TYPELESS, UNKNOWN,
                                  D24_UNORM_S8_UINT, R24_UNORM_X8_TYPELESS);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_X24_TYPELESS_G8_UINT, X24_TYPELESS_G8_UINT, UNKNOWN,
                                  D24_UNORM_S8_UINT, X24_TYPELESS_G8_UINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG8_TYPELESS, RG8_UNORM, RG8_UNORM, UNKNOWN, RG8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG8_UNORM, RG8_UNORM, RG8_UNORM, UNKNOWN, RG8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG8_UINT, RG8_UINT, RG8_UINT, UNKNOWN, RG8_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG8_SNORM, RG8_SNORM, RG8_SNORM, UNKNOWN, RG8_SNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG8_SINT, RG8_SINT, RG8_SINT, UNKNOWN, RG8_SINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R16_TYPELESS, R16_FLOAT, R16_FLOAT, UNKNOWN, R16_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R16_FLOAT, R16_FLOAT, R16_FLOAT, UNKNOWN, R16_FLOAT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_D16_UNORM, R16_UNORM, R16_UNORM, D16_UNORM, R16_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R16_UNORM, R16_UNORM, R16_UNORM, D16_UNORM, R16_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R16_UINT, R16_UINT, R16_UINT, UNKNOWN, R16_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R16_SNORM, R16_SNORM, R16_SNORM, UNKNOWN, R16_SNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R16_SINT, R16_SINT, R16_SINT, UNKNOWN, R16_SINT);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R8_TYPELESS, R8_UNORM, R8_UNORM, UNKNOWN, R8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R8_UNORM, R8_UNORM, R8_UNORM, UNKNOWN, R8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R8_UINT, R8_UINT, R8_UINT, UNKNOWN, R8_UINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R8_SNORM, R8_SNORM, R8_SNORM, UNKNOWN, R8_SNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R8_SINT, R8_SINT, R8_SINT, UNKNOWN, R8_SINT);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_A8_UNORM, A8_UNORM, A8_UNORM, UNKNOWN, A8_UNORM);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R1_UNORM, R1_UNORM, R1_UNORM, UNKNOWN, R1_UNORM);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RGB9E5_SHAREDEXP, RGB9E5_SHAREDEXP, RGB9E5_SHAREDEXP, UNKNOWN,
                                  RGB9E5_SHAREDEXP);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_RG8_B8G8_UNORM, RG8_B8G8_UNORM, RG8_B8G8_UNORM, UNKNOWN,
                                  RG8_B8G8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_G8R8_G8B8_UNORM, G8R8_G8B8_UNORM, G8R8_G8B8_UNORM, UNKNOWN,
                                  G8R8_G8B8_UNORM);

        // http://www.g-truc.net/post-0335.html
        // http://renderingpipeline.com/2012/07/texture-compression/
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC1_TYPELESS, BC1_UNORM_SRGB, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC1_UNORM, BC1_UNORM, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC1_UNORM_SRGB, BC1_UNORM_SRGB, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC2_TYPELESS, BC2_UNORM_SRGB, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC2_UNORM, BC2_UNORM, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC2_UNORM_SRGB, BC2_UNORM_SRGB, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC3_TYPELESS, BC3_UNORM_SRGB, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC3_UNORM, BC3_UNORM, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC3_UNORM_SRGB, BC3_UNORM_SRGB, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC4_TYPELESS, BC4_UNORM, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC4_UNORM, BC4_UNORM, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC4_SNORM, BC4_SNORM, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC5_TYPELESS, BC5_UNORM, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC5_UNORM, BC5_UNORM, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC5_SNORM, BC5_SNORM, UNKNOWN, UNKNOWN, UNKNOWN);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_B5G6R5_UNORM, B5G6R5_UNORM, B5G6R5_UNORM, UNKNOWN, B5G6R5_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_B5G5R5A1_UNORM, B5G5R5A1_UNORM, B5G5R5A1_UNORM, UNKNOWN,
                                  B5G5R5A1_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BGRA8_UNORM, BGRA8_UNORM, BGRA8_UNORM, UNKNOWN, BGRA8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BGRX8_UNORM, BGRX8_UNORM, BGRX8_UNORM, UNKNOWN, BGRX8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM, R10G10B10_XR_BIAS_A2_UNORM, UNKNOWN,
                                  UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BGRA8_TYPELESS, BGRA8_UNORM_SRGB, BGRA8_UNORM_SRGB, UNKNOWN,
                                  BGRA8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BGRA8_UNORM_SRGB, BGRA8_UNORM_SRGB, BGRA8_UNORM_SRGB, UNKNOWN,
                                  BGRA8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BGRX8_TYPELESS, BGRX8_UNORM_SRGB, BGRX8_UNORM_SRGB, UNKNOWN,
                                  BGRX8_UNORM);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BGRX8_UNORM_SRGB, BGRX8_UNORM_SRGB, BGRX8_UNORM_SRGB, UNKNOWN,
                                  BGRX8_UNORM);

        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC6H_TYPELESS, BC6H_UF16, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC6H_UF16, BC6H_UF16, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC6H_SF16, BC6H_SF16, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC7_TYPELESS, BC7_UNORM_SRGB, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC7_UNORM, BC7_UNORM, UNKNOWN, UNKNOWN, UNKNOWN);
        INIT_TEX_VIEW_FORMAT_INFO(TEX_FORMAT_BC7_UNORM_SRGB, BC7_UNORM_SRGB, UNKNOWN, UNKNOWN, UNKNOWN);
#undef INIT_TVIEW_FORMAT_INFO
    }

    TEXTURE_FORMAT GetViewFormat(TEXTURE_FORMAT format, TEXTURE_VIEW_TYPE view_type, uint32_t bind_flags)
    {
        assert(view_type > TEXTURE_VIEW_UNDEFINED
               && view_type < TEXTURE_VIEW_NUM_VIEWS);   // Unexpected texture view type
        assert(format >= TEX_FORMAT_UNKNOWN && format < TEX_FORMAT_NUM_FORMATS);   // Unknown texture format
        switch(format)
        {
            case TEX_FORMAT_R16_TYPELESS: {
                if(bind_flags & BIND_DEPTH_STENCIL)
                {
                    static TEXTURE_FORMAT D16_view_fmts[] = {TEX_FORMAT_R16_UNORM, TEX_FORMAT_R16_UNORM,
                                                             TEX_FORMAT_D16_UNORM, TEX_FORMAT_R16_UNORM};
                    return D16_view_fmts[view_type - 1];
                }
            }

            default: /*do nothing*/
                break;
        }

        return m_view_formats[format][view_type - 1];
    }

private:
    TEXTURE_FORMAT m_view_formats[TEX_FORMAT_NUM_FORMATS][TEXTURE_VIEW_NUM_VIEWS - 1];
};

TEXTURE_FORMAT GetDefaultTextureViewFormat(TEXTURE_FORMAT texture_format, TEXTURE_VIEW_TYPE view_type,
                                           uint32_t bind_flags)
{
    static TexFormatToViewFormatConverter fmt_converter;
    return fmt_converter.GetViewFormat(texture_format, view_type, bind_flags);
}

const TextureFormatInfo & GetTextureFormatInfo(TEXTURE_FORMAT format)
{
    static TextureFormatInfo fmt_infos[TEX_FORMAT_NUM_FORMATS];
    static bool              is_init = false;

    // Note that this implementation is thread-safe
    // Even if two threads try to call the function at the same time,
    // the worst thing that might happen is that the array will be
    // initialized multiple times. But the result will always be correct.
    if(!is_init)
    {
#define INIT_TEX_FORMAT_INFO(tex_fmt, component_size, num_components, component_type, is_typeless, \
                             block_width, block_height)                                            \
    fmt_infos[tex_fmt] = TextureFormatInfo(#tex_fmt, tex_fmt, component_size, num_components,      \
                                           component_type, is_typeless, block_width, block_height);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA32_TYPELESS, 4, 4, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA32_FLOAT, 4, 4, COMPONENT_TYPE_FLOAT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA32_UINT, 4, 4, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA32_SINT, 4, 4, COMPONENT_TYPE_SINT, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGB32_TYPELESS, 4, 3, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGB32_FLOAT, 4, 3, COMPONENT_TYPE_FLOAT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGB32_UINT, 4, 3, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGB32_SINT, 4, 3, COMPONENT_TYPE_SINT, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA16_TYPELESS, 2, 4, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA16_FLOAT, 2, 4, COMPONENT_TYPE_FLOAT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA16_UNORM, 2, 4, COMPONENT_TYPE_UNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA16_UINT, 2, 4, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA16_SNORM, 2, 4, COMPONENT_TYPE_SNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA16_SINT, 2, 4, COMPONENT_TYPE_SINT, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG32_TYPELESS, 4, 2, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG32_FLOAT, 4, 2, COMPONENT_TYPE_FLOAT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG32_UINT, 4, 2, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG32_SINT, 4, 2, COMPONENT_TYPE_SINT, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R32G8X24_TYPELESS, 4, 2, COMPONENT_TYPE_DEPTH_STENCIL, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_D32_FLOAT_S8X24_UINT, 4, 2, COMPONENT_TYPE_DEPTH_STENCIL, false, 1,
                             1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS, 4, 2, COMPONENT_TYPE_DEPTH_STENCIL, false,
                             1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_X32_TYPELESS_G8X24_UINT, 4, 2, COMPONENT_TYPE_DEPTH_STENCIL, false, 1,
                             1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGB10A2_TYPELESS, 4, 1, COMPONENT_TYPE_COMPOUND, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGB10A2_UNORM, 4, 1, COMPONENT_TYPE_COMPOUND, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGB10A2_UINT, 4, 1, COMPONENT_TYPE_COMPOUND, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R11G11B10_FLOAT, 4, 1, COMPONENT_TYPE_COMPOUND, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA8_TYPELESS, 1, 4, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA8_UNORM, 1, 4, COMPONENT_TYPE_UNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA8_UNORM_SRGB, 1, 4, COMPONENT_TYPE_UNORM_SRGB, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA8_UINT, 1, 4, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA8_SNORM, 1, 4, COMPONENT_TYPE_SNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGBA8_SINT, 1, 4, COMPONENT_TYPE_SINT, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG16_TYPELESS, 2, 2, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG16_FLOAT, 2, 2, COMPONENT_TYPE_FLOAT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG16_UNORM, 2, 2, COMPONENT_TYPE_UNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG16_UINT, 2, 2, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG16_SNORM, 2, 2, COMPONENT_TYPE_SNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG16_SINT, 2, 2, COMPONENT_TYPE_SINT, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R32_TYPELESS, 4, 1, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_D32_FLOAT, 4, 1, COMPONENT_TYPE_DEPTH, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R32_FLOAT, 4, 1, COMPONENT_TYPE_FLOAT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R32_UINT, 4, 1, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R32_SINT, 4, 1, COMPONENT_TYPE_SINT, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R24G8_TYPELESS, 4, 1, COMPONENT_TYPE_DEPTH_STENCIL, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_D24_UNORM_S8_UINT, 4, 1, COMPONENT_TYPE_DEPTH_STENCIL, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R24_UNORM_X8_TYPELESS, 4, 1, COMPONENT_TYPE_DEPTH_STENCIL, false, 1,
                             1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_X24_TYPELESS_G8_UINT, 4, 1, COMPONENT_TYPE_DEPTH_STENCIL, false, 1,
                             1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG8_TYPELESS, 1, 2, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG8_UNORM, 1, 2, COMPONENT_TYPE_UNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG8_UINT, 1, 2, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG8_SNORM, 1, 2, COMPONENT_TYPE_SNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG8_SINT, 1, 2, COMPONENT_TYPE_SINT, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R16_TYPELESS, 2, 1, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R16_FLOAT, 2, 1, COMPONENT_TYPE_FLOAT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_D16_UNORM, 2, 1, COMPONENT_TYPE_DEPTH, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R16_UNORM, 2, 1, COMPONENT_TYPE_UNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R16_UINT, 2, 1, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R16_SNORM, 2, 1, COMPONENT_TYPE_SNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R16_SINT, 2, 1, COMPONENT_TYPE_SINT, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R8_TYPELESS, 1, 1, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R8_UNORM, 1, 1, COMPONENT_TYPE_UNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R8_UINT, 1, 1, COMPONENT_TYPE_UINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R8_SNORM, 1, 1, COMPONENT_TYPE_SNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R8_SINT, 1, 1, COMPONENT_TYPE_SINT, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_A8_UNORM, 1, 1, COMPONENT_TYPE_UNORM, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R1_UNORM, 1, 1, COMPONENT_TYPE_UNORM, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RGB9E5_SHAREDEXP, 4, 1, COMPONENT_TYPE_COMPOUND, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_RG8_B8G8_UNORM, 1, 4, COMPONENT_TYPE_UNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_G8R8_G8B8_UNORM, 1, 4, COMPONENT_TYPE_UNORM, false, 1, 1);

        // http://www.g-truc.net/post-0335.html
        // http://renderingpipeline.com/2012/07/texture-compression/
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC1_TYPELESS, 8, 3, COMPONENT_TYPE_COMPRESSED, true, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC1_UNORM, 8, 3, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC1_UNORM_SRGB, 8, 3, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC2_TYPELESS, 16, 4, COMPONENT_TYPE_COMPRESSED, true, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC2_UNORM, 16, 4, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC2_UNORM_SRGB, 16, 4, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC3_TYPELESS, 16, 4, COMPONENT_TYPE_COMPRESSED, true, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC3_UNORM, 16, 4, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC3_UNORM_SRGB, 16, 4, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC4_TYPELESS, 8, 1, COMPONENT_TYPE_COMPRESSED, true, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC4_UNORM, 8, 1, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC4_SNORM, 8, 1, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC5_TYPELESS, 16, 2, COMPONENT_TYPE_COMPRESSED, true, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC5_UNORM, 16, 2, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC5_SNORM, 16, 2, COMPONENT_TYPE_COMPRESSED, false, 4, 4);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_B5G6R5_UNORM, 2, 1, COMPONENT_TYPE_COMPOUND, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_B5G5R5A1_UNORM, 2, 1, COMPONENT_TYPE_COMPOUND, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BGRA8_UNORM, 1, 4, COMPONENT_TYPE_UNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BGRX8_UNORM, 1, 4, COMPONENT_TYPE_UNORM, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM, 4, 1, COMPONENT_TYPE_COMPOUND, false, 1,
                             1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BGRA8_TYPELESS, 1, 4, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BGRA8_UNORM_SRGB, 1, 4, COMPONENT_TYPE_UNORM_SRGB, false, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BGRX8_TYPELESS, 1, 4, COMPONENT_TYPE_UNDEFINED, true, 1, 1);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BGRX8_UNORM_SRGB, 1, 4, COMPONENT_TYPE_UNORM_SRGB, false, 1, 1);

        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC6H_TYPELESS, 16, 3, COMPONENT_TYPE_COMPRESSED, true, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC6H_UF16, 16, 3, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC6H_SF16, 16, 3, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC7_TYPELESS, 16, 4, COMPONENT_TYPE_COMPRESSED, true, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC7_UNORM, 16, 4, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
        INIT_TEX_FORMAT_INFO(TEX_FORMAT_BC7_UNORM_SRGB, 16, 4, COMPONENT_TYPE_COMPRESSED, false, 4, 4);
#undef INIT_TEX_FORMAT_INFO
        static_assert(TEX_FORMAT_NUM_FORMATS == TEX_FORMAT_BC7_UNORM_SRGB + 1,
                      "Not all texture formats initialized.");

#ifdef DEBUG
        for(uint32_t fmt = TEX_FORMAT_UNKNOWN; fmt < TEX_FORMAT_NUM_FORMATS; ++fmt)
            assert(fmt_infos[fmt].format == static_cast<TEXTURE_FORMAT>(fmt));   // Uninitialized format
#endif

        is_init = true;
    }

    if(format >= TEX_FORMAT_UNKNOWN && format < TEX_FORMAT_NUM_FORMATS)
    {
        const auto & info = fmt_infos[format];
        assert(info.format == format);   // Unexpected format
        return info;
    }
    else
    {
        UNEXPECTED("Texture format (", int{format}, ") is out of allowed range [0, ",
                   int{TEX_FORMAT_NUM_FORMATS} - 1, "]");
        return fmt_infos[0];
    }
}

const char * GetTexViewTypeLiteralName(TEXTURE_VIEW_TYPE view_type)
{
    static const char * tex_view_literal_names[TEXTURE_VIEW_NUM_VIEWS] = {};
    static bool         is_init                                        = false;
    // Note that this implementation is thread-safe
    // Even if two threads try to call the function at the same time,
    // the worst thing that might happen is that the array will be
    // initialized multiple times. But the result will always be correct.
    if(!is_init)
    {
#define INIT_TEX_VIEW_TYPE_NAME(view_type) tex_view_literal_names[view_type] = #view_type
        INIT_TEX_VIEW_TYPE_NAME(TEXTURE_VIEW_UNDEFINED);
        INIT_TEX_VIEW_TYPE_NAME(TEXTURE_VIEW_SHADER_RESOURCE);
        INIT_TEX_VIEW_TYPE_NAME(TEXTURE_VIEW_RENDER_TARGET);
        INIT_TEX_VIEW_TYPE_NAME(TEXTURE_VIEW_DEPTH_STENCIL);
        INIT_TEX_VIEW_TYPE_NAME(TEXTURE_VIEW_UNORDERED_ACCESS);
#undef INIT_TEX_VIEW_TYPE_NAME
        static_assert(TEXTURE_VIEW_NUM_VIEWS == TEXTURE_VIEW_UNORDERED_ACCESS + 1,
                      "Not all texture views names initialized.");

        is_init = true;
    }

    if(view_type >= TEXTURE_VIEW_UNDEFINED && view_type < TEXTURE_VIEW_NUM_VIEWS)
    {
        return tex_view_literal_names[view_type];
    }
    else
    {
        UNEXPECTED("Texture view type (", view_type, ") is out of allowed range [0, ",
                   TEXTURE_VIEW_NUM_VIEWS - 1, "]");
        return "<Unknown texture view type>";
    }
}

const char * GetBufferViewTypeLiteralName(BUFFER_VIEW_TYPE view_type)
{
    static const char * buff_view_literal_names[BUFFER_VIEW_NUM_VIEWS] = {};
    static bool         is_init                                        = false;
    // Note that this implementation is thread-safe
    // Even if two threads try to call the function at the same time,
    // the worst thing that might happen is that the array will be
    // initialized multiple times. But the result will always be correct.
    if(!is_init)
    {
#define INIT_BUFF_VIEW_TYPE_NAME(view_type) buff_view_literal_names[view_type] = #view_type
        INIT_BUFF_VIEW_TYPE_NAME(BUFFER_VIEW_UNDEFINED);
        INIT_BUFF_VIEW_TYPE_NAME(BUFFER_VIEW_SHADER_RESOURCE);
        INIT_BUFF_VIEW_TYPE_NAME(BUFFER_VIEW_UNORDERED_ACCESS);
#undef INIT_BUFF_VIEW_TYPE_NAME
        static_assert(BUFFER_VIEW_NUM_VIEWS == BUFFER_VIEW_UNORDERED_ACCESS + 1,
                      "Not all buffer views names initialized.");

        is_init = true;
    }

    if(view_type >= BUFFER_VIEW_UNDEFINED && view_type < BUFFER_VIEW_NUM_VIEWS)
    {
        return buff_view_literal_names[view_type];
    }
    else
    {
        UNEXPECTED("Buffer view type (", view_type, ") is out of allowed range [0, ",
                   BUFFER_VIEW_NUM_VIEWS - 1, "]");
        return "<Unknown buffer view type>";
    }
}

const char * GetShaderTypeLiteralName(SHADER_TYPE shader_type)
{
    switch(shader_type)
    {
#define RETURN_SHADER_TYPE_NAME(shader_type) \
    case shader_type:                        \
        return #shader_type;

        RETURN_SHADER_TYPE_NAME(SHADER_TYPE_UNKNOWN)
        RETURN_SHADER_TYPE_NAME(SHADER_TYPE_VERTEX)
        RETURN_SHADER_TYPE_NAME(SHADER_TYPE_PIXEL)
        RETURN_SHADER_TYPE_NAME(SHADER_TYPE_GEOMETRY)
        RETURN_SHADER_TYPE_NAME(SHADER_TYPE_HULL)
        RETURN_SHADER_TYPE_NAME(SHADER_TYPE_DOMAIN)
        RETURN_SHADER_TYPE_NAME(SHADER_TYPE_COMPUTE)
#undef RETURN_SHADER_TYPE_NAME

        default:
            UNEXPECTED("Unknown shader type constant ", uint32_t{shader_type});
            return "<Unknown shader type>";
    }
}

std::string GetShaderStagesString(SHADER_TYPE shader_stages)
{
    std::string stages_str;
    while(shader_stages != 0)
        for(uint32_t stage = SHADER_TYPE_VERTEX; shader_stages != 0 && stage <= SHADER_TYPE_COMPUTE;
            stage <<= 1)
        {
            if(shader_stages & stage)
            {
                if(stages_str.length())
                    stages_str += ", ";
                stages_str += GetShaderTypeLiteralName(static_cast<SHADER_TYPE>(stage));
                shader_stages = static_cast<SHADER_TYPE>(shader_stages & (~stage));
            }
        }
    assert(shader_stages == 0);
    return stages_str;
}

const char * GetShaderVariableTypeLiteralName(SHADER_RESOURCE_VARIABLE_TYPE var_type, bool get_full_name)
{
    static const char * short_var_type_name_strings[SHADER_RESOURCE_VARIABLE_TYPE_NUM_TYPES];
    static const char * full_var_type_name_strings[SHADER_RESOURCE_VARIABLE_TYPE_NUM_TYPES];
    static bool         var_type_strs_init = false;
    if(!var_type_strs_init)
    {
        short_var_type_name_strings[SHADER_RESOURCE_VARIABLE_TYPE_STATIC]  = "static";
        short_var_type_name_strings[SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE] = "mutable";
        short_var_type_name_strings[SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC] = "dynamic";
        full_var_type_name_strings[SHADER_RESOURCE_VARIABLE_TYPE_STATIC] =
            "SHADER_RESOURCE_VARIABLE_TYPE_STATIC";
        full_var_type_name_strings[SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE] =
            "SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE";
        full_var_type_name_strings[SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC] =
            "SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC";

        static_assert(SHADER_RESOURCE_VARIABLE_TYPE_NUM_TYPES == SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC + 1,
                      "Not all shader variable types initialized.");

        var_type_strs_init = true;
    }
    if(var_type >= SHADER_RESOURCE_VARIABLE_TYPE_STATIC && var_type < SHADER_RESOURCE_VARIABLE_TYPE_NUM_TYPES)
        return (get_full_name ? full_var_type_name_strings : short_var_type_name_strings)[var_type];
    else
    {
        UNEXPECTED("Unknow shader variable type");
        return "unknow";
    }
}

const char * GetShaderResourceTypeLiteralName(SHADER_RESOURCE_TYPE resource_type, bool get_full_name)
{
    switch(resource_type)
    {
        case SHADER_RESOURCE_TYPE_UNKNOWN:
            return get_full_name ? "SHADER_RESOURCE_TYPE_UNKNOWN" : "unknown";
        case SHADER_RESOURCE_TYPE_CONSTANT_BUFFER:
            return get_full_name ? "SHADER_RESOURCE_TYPE_CONSTANT_BUFFER" : "constant buffer";
        case SHADER_RESOURCE_TYPE_TEXTURE_SRV:
            return get_full_name ? "SHADER_RESOURCE_TYPE_TEXTURE_SRV" : "texture SRV";
        case SHADER_RESOURCE_TYPE_BUFFER_SRV:
            return get_full_name ? "SHADER_RESOURCE_TYPE_BUFFER_SRV" : "buffer SRV";
        case SHADER_RESOURCE_TYPE_TEXTURE_UAV:
            return get_full_name ? "SHADER_RESOURCE_TYPE_TEXTURE_UAV" : "texture UAV";
        case SHADER_RESOURCE_TYPE_BUFFER_UAV:
            return get_full_name ? "SHADER_RESOURCE_TYPE_BUFFER_UAV" : "buffer UAV";
        case SHADER_RESOURCE_TYPE_SAMPLER:
            return get_full_name ? "SHADER_RESOURCE_TYPE_SAMPLER" : "sampler";
        default:
            UNEXPECTED("Unexepcted resource type (", uint32_t{resource_type}, ")");
            return "UNKNOWN";
    }
}

const char * GetMapTypeString(MAP_TYPE map_type)
{
    switch(map_type)
    {
        case MAP_READ:
            return "MAP_READ";
        case MAP_WRITE:
            return "MAP_WRITE";
        case MAP_READ_WRITE:
            return "MAP_READ_WRITE";

        default:
            UNEXPECTED("Unexpected map type");
            return "Unknown map type";
    }
}

/// Returns the string containing the usage
const char * GetUsageString(USAGE usage)
{
    static const char * usage_strings[4];
    static bool         usage_strings_init = false;
    if(!usage_strings_init)
    {
#define INIT_USGAGE_STR(Usage) usage_strings[Usage] = #Usage
        INIT_USGAGE_STR(USAGE_STATIC);
        INIT_USGAGE_STR(USAGE_DEFAULT);
        INIT_USGAGE_STR(USAGE_DYNAMIC);
        INIT_USGAGE_STR(USAGE_CPU_ACCESSIBLE);
#undef INIT_USGAGE_STR
        usage_strings_init = true;
    }
    if(usage >= USAGE_STATIC && usage <= USAGE_CPU_ACCESSIBLE)
        return usage_strings[usage];
    else
    {
        UNEXPECTED("Unknow usage");
        return "Unknown usage";
    }
}

const char * GetResourceDimString(RESOURCE_DIMENSION tex_type)
{
    static const char * tex_type_strings[RESOURCE_DIM_NUM_DIMENSIONS];
    static bool         tex_type_strs_init = false;
    if(!tex_type_strs_init)
    {
        tex_type_strings[RESOURCE_DIM_UNDEFINED]      = "Undefined";
        tex_type_strings[RESOURCE_DIM_BUFFER]         = "Buffer";
        tex_type_strings[RESOURCE_DIM_TEX_1D]         = "Tex 1D";
        tex_type_strings[RESOURCE_DIM_TEX_1D_ARRAY]   = "Tex 1D Array";
        tex_type_strings[RESOURCE_DIM_TEX_2D]         = "Tex 2D";
        tex_type_strings[RESOURCE_DIM_TEX_2D_ARRAY]   = "Tex 2D Array";
        tex_type_strings[RESOURCE_DIM_TEX_3D]         = "Tex 3D";
        tex_type_strings[RESOURCE_DIM_TEX_CUBE]       = "Tex Cube";
        tex_type_strings[RESOURCE_DIM_TEX_CUBE_ARRAY] = "Tex Cube Array";
        static_assert(RESOURCE_DIM_NUM_DIMENSIONS == RESOURCE_DIM_TEX_CUBE_ARRAY + 1,
                      "Not all texture type strings initialized.");

        tex_type_strs_init = true;
    }
    if(tex_type >= RESOURCE_DIM_UNDEFINED && tex_type < RESOURCE_DIM_NUM_DIMENSIONS)
        return tex_type_strings[tex_type];
    else
    {
        UNEXPECTED("Unknow texture type");
        return "Unknow texture type";
    }
}

const char * GetBindFlagString(uint32_t bind_flag)
{
    assert((bind_flag & (bind_flag - 1)) == 0);   // More than one bind flag specified
    switch(bind_flag)
    {
#define BIND_FLAG_STR_CASE(Flag) \
    case Flag:                   \
        return #Flag;
        BIND_FLAG_STR_CASE(BIND_VERTEX_BUFFER)
        BIND_FLAG_STR_CASE(BIND_INDEX_BUFFER)
        BIND_FLAG_STR_CASE(BIND_UNIFORM_BUFFER)
        BIND_FLAG_STR_CASE(BIND_SHADER_RESOURCE)
        BIND_FLAG_STR_CASE(BIND_STREAM_OUTPUT)
        BIND_FLAG_STR_CASE(BIND_RENDER_TARGET)
        BIND_FLAG_STR_CASE(BIND_DEPTH_STENCIL)
        BIND_FLAG_STR_CASE(BIND_UNORDERED_ACCESS)
        BIND_FLAG_STR_CASE(BIND_INDIRECT_DRAW_ARGS)
#undef BIND_FLAG_STR_CASE
        default:
            UNEXPECTED("Unexpected bind flag ", bind_flag);
            return "";
    }
}

std::string GetBindFlagsString(uint32_t bind_flags)
{
    if(bind_flags == 0)
        return "0";
    std::string str;
    for(uint32_t flag = BIND_VERTEX_BUFFER; bind_flags && flag <= BIND_INDIRECT_DRAW_ARGS; flag <<= 1)
    {
        if(bind_flags & flag)
        {
            if(str.length())
                str += '|';
            str += GetBindFlagString(flag);
            bind_flags &= ~flag;
        }
    }
    assert(bind_flags == 0);   // Unknown bind flags left
    return str;
}

static const char * GetSingleCPUAccessFlagString(uint32_t cpu_access_flag)
{
    assert((cpu_access_flag & (cpu_access_flag - 1)) == 0);   // More than one access flag specified
    switch(cpu_access_flag)
    {
#define CPU_ACCESS_FLAG_STR_CASE(Flag) \
    case Flag:                         \
        return #Flag;
        CPU_ACCESS_FLAG_STR_CASE(CPU_ACCESS_READ)
        CPU_ACCESS_FLAG_STR_CASE(CPU_ACCESS_WRITE)
#undef CPU_ACCESS_FLAG_STR_CASE
        default:
            UNEXPECTED("Unexpected CPU access flag ", cpu_access_flag);
            return "";
    }
}

std::string GetCPUAccessFlagsString(uint32_t cpu_access_flags)
{
    if(cpu_access_flags == 0)
        return "0";
    std::string str;
    for(uint32_t flag = CPU_ACCESS_READ; cpu_access_flags && flag <= CPU_ACCESS_WRITE; flag <<= 1)
    {
        if(cpu_access_flags & flag)
        {
            if(str.length())
                str += '|';
            str += GetSingleCPUAccessFlagString(flag);
            cpu_access_flags &= ~flag;
        }
    }
    assert(cpu_access_flags == 0);   // Unknown CPU access flags left
    return str;
}

std::string GetTextureDescString(const TextureDesc & desc)
{
    std::string str = "Type: ";
    str += GetResourceDimString(desc.type);
    str += "; size: ";
    str += std::to_string(desc.width);
    if(desc.type == RESOURCE_DIM_TEX_2D || desc.type == RESOURCE_DIM_TEX_2D_ARRAY
       || desc.type == RESOURCE_DIM_TEX_3D || desc.type == RESOURCE_DIM_TEX_CUBE
       || desc.type == RESOURCE_DIM_TEX_CUBE_ARRAY)
    {
        str += "x";
        str += std::to_string(desc.height);
    }

    if(desc.type == RESOURCE_DIM_TEX_3D)
    {
        str += "x";
        str += std::to_string(desc.depth);
    }

    if(desc.type == RESOURCE_DIM_TEX_1D_ARRAY || desc.type == RESOURCE_DIM_TEX_2D_ARRAY
       || desc.type == RESOURCE_DIM_TEX_CUBE || desc.type == RESOURCE_DIM_TEX_CUBE_ARRAY)
    {
        str += "; Num Slices: ";
        str += std::to_string(desc.array_size);
    }

    auto fmt_name = GetTextureFormatInfo(desc.format).name;
    str += "; Format: ";
    str += fmt_name;

    str += "; Mip levels: ";
    str += std::to_string(desc.mip_levels);

    str += "; Sample Count: ";
    str += std::to_string(desc.sample_count);

    str += "; Usage: ";
    str += GetUsageString(desc.usage);

    str += "; Bind Flags: ";
    str += GetBindFlagsString(desc.bind_flags);

    str += "; CPU access: ";
    str += GetCPUAccessFlagsString(desc.cpu_access_flags);

    return str;
}

const char * GetBufferModeString(BUFFER_MODE mode)
{
    static const char * buffer_mode_strings[BUFFER_MODE_NUM_MODES];
    static bool         buff_mode_strings_init = false;
    if(!buff_mode_strings_init)
    {
#define INIT_BUFF_MODE_STR(Mode) buffer_mode_strings[Mode] = #Mode
        INIT_BUFF_MODE_STR(BUFFER_MODE_UNDEFINED);
        INIT_BUFF_MODE_STR(BUFFER_MODE_FORMATTED);
        INIT_BUFF_MODE_STR(BUFFER_MODE_STRUCTURED);
        INIT_BUFF_MODE_STR(BUFFER_MODE_RAW);
#undef INIT_BUFF_MODE_STR
        static_assert(BUFFER_MODE_NUM_MODES == BUFFER_MODE_RAW + 1,
                      "Not all buffer mode strings initialized.");
        buff_mode_strings_init = true;
    }
    if(mode >= BUFFER_MODE_UNDEFINED && mode < BUFFER_MODE_NUM_MODES)
        return buffer_mode_strings[mode];
    else
    {
        UNEXPECTED("Unknown buffer mode");
        return "Unknown buffer mode";
    }
}

std::string GetBufferFormatString(const BufferFormat & fmt)
{
    std::string str;
    str += GetValueTypeString(fmt.value_type);
    if(fmt.is_normalized)
        str += " norm";
    str += " x ";
    str += std::to_string(uint32_t{fmt.num_components});
    return str;
}

std::string GetBufferDescString(const BufferDesc & desc)
{
    std::string str;
    str += "Size: ";
    bool is_large = false;
    if(desc.size_in_bytes > (1 << 20))
    {
        str += std::to_string(desc.size_in_bytes / (1 << 20));
        str += " Mb (";
        is_large = true;
    }
    else if(desc.size_in_bytes > (1 << 10))
    {
        str += std::to_string(desc.size_in_bytes / (1 << 10));
        str += " Kb (";
        is_large = true;
    }

    str += std::to_string(desc.size_in_bytes);
    str += " bytes";
    if(is_large)
        str += ')';

    str += "; Mode: ";
    str += GetBufferModeString(desc.mode);

    str += "; Usage: ";
    str += GetUsageString(desc.usage);

    str += "; Bind Flags: ";
    str += GetBindFlagsString(desc.bind_flags);

    str += "; CPU access: ";
    str += GetCPUAccessFlagsString(desc.cpu_access_flags);

    str += "; stride: ";
    str += std::to_string(desc.element_byte_stride);
    str += " bytes";

    return str;
}

const char * GetResourceStateFlagString(RESOURCE_STATE state)
{
    assert((state & (state - 1)) == 0);   // Single state is expected
    switch(state)
    {
        case RESOURCE_STATE_UNKNOWN:
            return "UNKNOWN";
        case RESOURCE_STATE_UNDEFINED:
            return "UNDEFINED";
        case RESOURCE_STATE_VERTEX_BUFFER:
            return "VERTEX_BUFFER";
        case RESOURCE_STATE_CONSTANT_BUFFER:
            return "CONSTANT_BUFFER";
        case RESOURCE_STATE_INDEX_BUFFER:
            return "INDEX_BUFFER";
        case RESOURCE_STATE_RENDER_TARGET:
            return "RENDER_TARGET";
        case RESOURCE_STATE_UNORDERED_ACCESS:
            return "UNORDERED_ACCESS";
        case RESOURCE_STATE_DEPTH_WRITE:
            return "DEPTH_WRITE";
        case RESOURCE_STATE_DEPTH_READ:
            return "DEPTH_READ";
        case RESOURCE_STATE_SHADER_RESOURCE:
            return "SHADER_RESOURCE";
        case RESOURCE_STATE_STREAM_OUT:
            return "STREAM_OUT";
        case RESOURCE_STATE_INDIRECT_ARGUMENT:
            return "INDIRECT_ARGUMENT";
        case RESOURCE_STATE_COPY_DEST:
            return "COPY_DEST";
        case RESOURCE_STATE_COPY_SOURCE:
            return "COPY_SOURCE";
        case RESOURCE_STATE_RESOLVE_DEST:
            return "RESOLVE_DEST";
        case RESOURCE_STATE_RESOLVE_SOURCE:
            return "RESOLVE_SOURCE";
        case RESOURCE_STATE_PRESENT:
            return "PRESENT";
        default:
            UNEXPECTED("Unknown resource state");
            return "UNKNOWN";
    }
}

std::string GetResourceStateString(RESOURCE_STATE state)
{
    if(state == RESOURCE_STATE_UNKNOWN)
        return "UNKNOWN";

    std::string str;
    while(state != 0)
    {
        if(!str.empty())
            str.push_back('|');

        auto         lsb             = state & ~(state - 1);
        const auto * StateFlagString = GetResourceStateFlagString(static_cast<RESOURCE_STATE>(lsb));
        str.append(StateFlagString);
        state = static_cast<RESOURCE_STATE>(state & ~lsb);
    }
    return str;
}

uint32_t ComputeMipLevelsCount(uint32_t width)
{
    if(width == 0)
        return 0;

    uint32_t mip_levels = 0;
    while((width >> mip_levels) > 0)
        ++mip_levels;
    assert(width >= (1U << (mip_levels - 1))
           && width < (1U << mip_levels));   // Incorrect number of Mip levels
    return mip_levels;
}

uint32_t ComputeMipLevelsCount(uint32_t width, uint32_t height)
{
    return ComputeMipLevelsCount(std::max(width, height));
}

uint32_t ComputeMipLevelsCount(uint32_t width, uint32_t height, uint32_t depth)
{
    return ComputeMipLevelsCount(std::max(std::max(width, height), depth));
}

bool VerifyResourceStates(RESOURCE_STATE state, bool is_texture)
{
    static_assert(RESOURCE_STATE_MAX_BIT == 0x8000,
                  "Please update this function to handle the new resource state");

#define VERIFY_EXCLUSIVE_STATE(ExclusiveState)                                                \
    if((state & ExclusiveState) != 0 && (state & ~ExclusiveState) != 0)                       \
    {                                                                                         \
        std::ostringstream ss;                                                                \
        ss << "State " << GetResourceStateString(state) << " is invalid: " << #ExclusiveState \
           << " can't be combined with any other state";                                      \
        Log::Log(Log::error, ss.str());                                                       \
        return false;                                                                         \
    }

    VERIFY_EXCLUSIVE_STATE(RESOURCE_STATE_UNDEFINED);
    VERIFY_EXCLUSIVE_STATE(RESOURCE_STATE_UNORDERED_ACCESS);
    VERIFY_EXCLUSIVE_STATE(RESOURCE_STATE_RENDER_TARGET);
    VERIFY_EXCLUSIVE_STATE(RESOURCE_STATE_DEPTH_WRITE);
    VERIFY_EXCLUSIVE_STATE(RESOURCE_STATE_COPY_DEST);
    VERIFY_EXCLUSIVE_STATE(RESOURCE_STATE_RESOLVE_DEST);
    VERIFY_EXCLUSIVE_STATE(RESOURCE_STATE_PRESENT);
#undef VERIFY_EXCLUSIVE_STATE

    if(is_texture)
    {
        if(state
           & (RESOURCE_STATE_VERTEX_BUFFER | RESOURCE_STATE_CONSTANT_BUFFER | RESOURCE_STATE_INDEX_BUFFER
              | RESOURCE_STATE_STREAM_OUT | RESOURCE_STATE_INDIRECT_ARGUMENT))
        {
            std::ostringstream ss;
            ss << "State " << GetResourceStateString(state)
               << " is invalid: states RESOURCE_STATE_VERTEX_BUFFER, "
               << "RESOURCE_STATE_CONSTANT_BUFFER, RESOURCE_STATE_INDEX_BUFFER, RESOURCE_STATE_STREAM_OUT, "
               << "RESOURCE_STATE_INDIRECT_ARGUMENT are not applicable to a texture";
            Log::Log(Log::error, ss.str());
            return false;
        }
    }
    else
    {
        if(state
           & (RESOURCE_STATE_RENDER_TARGET | RESOURCE_STATE_DEPTH_WRITE | RESOURCE_STATE_DEPTH_READ
              | RESOURCE_STATE_RESOLVE_SOURCE | RESOURCE_STATE_RESOLVE_DEST | RESOURCE_STATE_PRESENT))
        {
            std::ostringstream ss;
            ss << "State " << GetResourceStateString(state)
               << " is invalid: states RESOURCE_STATE_RENDER_TARGET, "
               << "RESOURCE_STATE_DEPTH_WRITE, RESOURCE_STATE_DEPTH_READ, RESOURCE_STATE_RESOLVE_SOURCE, "
               << "RESOURCE_STATE_RESOLVE_DEST, RESOURCE_STATE_PRESENT are not applicable to a buffer";
            Log::Log(Log::error, ss.str());
            return false;
        }
    }

    return true;
}
template<typename T>
bool IsPowerOfTwo(T val)
{
    return val > 0 && (val & (val - 1)) == 0;
}

template<typename T>
inline T Align(T val, T alignment)
{
    assert(IsPowerOfTwo(alignment));   // Alignment must be power of 2
    return (val + (alignment - 1)) & ~(alignment - 1);
}

MipLevelProperties GetMipLevelProperties(const TextureDesc & tex_desc, uint32_t mip_level)
{
    MipLevelProperties mip_props;
    const auto &       fmt_attribs = GetTextureFormatInfo(tex_desc.format);

    mip_props.logical_width  = std::max(tex_desc.width >> mip_level, 1u);
    mip_props.logical_height = std::max(tex_desc.height >> mip_level, 1u);
    mip_props.depth = (tex_desc.type == RESOURCE_DIM_TEX_3D) ? std::max(tex_desc.depth >> mip_level, 1u) : 1u;
    if(fmt_attribs.component_type == COMPONENT_TYPE_COMPRESSED)
    {
        assert(fmt_attribs.block_width > 1 && fmt_attribs.block_height > 1);
        assert((fmt_attribs.block_width & (fmt_attribs.block_width - 1))
               == 0);   // Compressed block width is expected to be power of 2
        assert((fmt_attribs.block_height & (fmt_attribs.block_height - 1))
               == 0);   // Compressed block height is expected to be power of 2

        // For block-compression formats, all parameters are still specified in texels rather than compressed
        // texel blocks (18.4.1)
        mip_props.storage_width  = Align(mip_props.logical_width, uint32_t{fmt_attribs.block_width});
        mip_props.storage_height = Align(mip_props.logical_height, uint32_t{fmt_attribs.block_height});
        mip_props.row_size       = mip_props.storage_width / uint32_t{fmt_attribs.block_width}
                             * uint32_t{fmt_attribs.component_size};   // ComponentSize is the block size
        mip_props.depth_slice_size =
            mip_props.storage_height / uint32_t{fmt_attribs.block_height} * mip_props.row_size;
        mip_props.mip_size = mip_props.depth_slice_size * mip_props.depth;
    }
    else
    {
        mip_props.storage_width  = mip_props.logical_width;
        mip_props.storage_height = mip_props.logical_height;
        mip_props.row_size       = mip_props.storage_width * uint32_t{fmt_attribs.component_size}
                             * uint32_t{fmt_attribs.num_components};
        mip_props.depth_slice_size = mip_props.row_size * mip_props.storage_height;
        mip_props.mip_size         = mip_props.depth_slice_size * mip_props.depth;
    }

    return mip_props;
}
}   // namespace evnt
