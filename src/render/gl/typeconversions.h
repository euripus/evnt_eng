#ifndef TYPECONVERSIONS_H
#define TYPECONVERSIONS_H

#include "../../log/debug_messages.h"
#include "../blendstate.h"
#include "../depthstencilstate.h"
#include <GL/glew.h>
#include <cassert>
#include <iterator>

namespace evnt
{
static const GLenum s_prim_topology_to_gltopology_map[] = {
    0,                   // PRIMITIVE_TOPOLOGY_UNDEFINED = 0
    GL_TRIANGLES,        // PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    GL_TRIANGLE_STRIP,   // PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    GL_POINTS,           // PRIMITIVE_TOPOLOGY_POINT_LIST
    GL_LINES             // PRIMITIVE_TOPOLOGY_LINE_LIST
};

inline GLenum PrimitiveTopologyToGLTopology(PRIMITIVE_TOPOLOGY prim_topology)
{
    assert(prim_topology < std::size(s_prim_topology_to_gltopology_map));
    auto GLTopology = s_prim_topology_to_gltopology_map[prim_topology];

    return GLTopology;
}

static const GLenum s_type_to_gltype_map[] = {
    0,                   // VT_UNDEFINED = 0
    GL_BYTE,             // VT_INT8
    GL_SHORT,            // VT_INT16
    GL_INT,              // VT_INT32
    GL_UNSIGNED_BYTE,    // VT_UINT8
    GL_UNSIGNED_SHORT,   // VT_UINT16
    GL_UNSIGNED_INT,     // VT_UINT32
    0,                   // VT_FLOAT16
    GL_FLOAT             // VT_FLOAT32
};

inline GLenum TypeToGLType(VALUE_TYPE value)
{
    assert(value < std::size(s_type_to_gltype_map));
    auto GLType = s_type_to_gltype_map[value];

    return GLType;
}

inline GLenum UsageToGLUsage(USAGE usage)
{
    // http://www.informit.com/articles/article.aspx?p=2033340&seqNum=2
    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBufferData.xml
    switch(usage)
    {
            // STATIC: The data store contents will be modified once and used many times.
            // STREAM: The data store contents will be modified once and used at MOST a few times.
            // DYNAMIC: The data store contents will be modified repeatedly and used many times.

        case USAGE_STATIC:
            return GL_STATIC_DRAW;
        case USAGE_DEFAULT:
            return GL_STATIC_DRAW;
        case USAGE_DYNAMIC:
            return GL_DYNAMIC_DRAW;
        case USAGE_CPU_ACCESSIBLE:
            return GL_DYNAMIC_READ;
        default:
            UNEXPECTED("Unknow usage");
            return 0;
    }
}

inline void FilterTypeToGLFilterType(FILTER_TYPE filter, GLenum & gl_filter, bool & is_anisotropic,
                                     bool & is_comparison)
{
    switch(filter)
    {
        case FILTER_TYPE_UNKNOWN:
            UNEXPECTED("Unspecified filter type");
            is_anisotropic = false;
            is_comparison  = false;
            gl_filter      = GL_NEAREST;
            break;

        case FILTER_TYPE_POINT:
            is_anisotropic = false;
            is_comparison  = false;
            gl_filter      = GL_NEAREST;
            break;

        case FILTER_TYPE_LINEAR:
            is_anisotropic = false;
            is_comparison  = false;
            gl_filter      = GL_LINEAR;
            break;

        case FILTER_TYPE_ANISOTROPIC:
            is_anisotropic = true;
            is_comparison  = false;
            gl_filter      = GL_LINEAR;
            break;

        case FILTER_TYPE_COMPARISON_POINT:
            is_anisotropic = false;
            is_comparison  = true;
            gl_filter      = GL_NEAREST;
            break;

        case FILTER_TYPE_COMPARISON_LINEAR:
            is_anisotropic = false;
            is_comparison  = true;
            gl_filter      = GL_LINEAR;
            break;

        case FILTER_TYPE_COMPARISON_ANISOTROPIC:
            is_anisotropic = true;
            is_comparison  = true;
            gl_filter      = GL_LINEAR;
            break;

        default:
            is_anisotropic = false;
            is_comparison  = false;
            UNEXPECTED("Unknown filter type");
            gl_filter = GL_NEAREST;
            break;
    }
}

inline GLenum TexAddressModeToGLAddressMode(TEXTURE_ADDRESS_MODE mode)
{
    switch(mode)
    {
        case TEXTURE_ADDRESS_UNKNOWN:
            UNEXPECTED("Texture address mode is not specified");
            return GL_CLAMP_TO_EDGE;
        case TEXTURE_ADDRESS_WRAP:
            return GL_REPEAT;
        case TEXTURE_ADDRESS_MIRROR:
            return GL_MIRRORED_REPEAT;
        case TEXTURE_ADDRESS_CLAMP:
            return GL_CLAMP_TO_EDGE;
        case TEXTURE_ADDRESS_BORDER:
            return GL_CLAMP_TO_BORDER;
        case TEXTURE_ADDRESS_MIRROR_ONCE:
            return GL_MIRROR_CLAMP_TO_EDGE;   // only available with OpenGL 4.4
            // This mode seems to be different from D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
            // The texture coord is clamped to the [-1, 1] range, but mirrors the
            // negative direction with the positive. Basically, it acts as
            // GL_CLAMP_TO_EDGE except that it takes the absolute value of the texture
            // coordinates before clamping.
        default:
            UNEXPECTED("Unknown texture address mode");
            return GL_CLAMP_TO_EDGE;
    }
}

inline GLenum CompareFuncToGLCompareFunc(COMPARISON_FUNCTION func)
{
    switch(func)
    {
        case COMPARISON_FUNC_UNKNOWN:
            UNEXPECTED("Comparison function is not specified");
            return GL_ALWAYS;
        case COMPARISON_FUNC_NEVER:
            return GL_NEVER;
        case COMPARISON_FUNC_LESS:
            return GL_LESS;
        case COMPARISON_FUNC_EQUAL:
            return GL_EQUAL;
        case COMPARISON_FUNC_LESS_EQUAL:
            return GL_LEQUAL;
        case COMPARISON_FUNC_GREATER:
            return GL_GREATER;
        case COMPARISON_FUNC_NOT_EQUAL:
            return GL_NOTEQUAL;
        case COMPARISON_FUNC_GREATER_EQUAL:
            return GL_GEQUAL;
        case COMPARISON_FUNC_ALWAYS:
            return GL_ALWAYS;
        default:
            UNEXPECTED("Unknown comparison func");
            return GL_ALWAYS;
    }
}

struct NativePixelAttribs
{
    GLenum pixel_format;
    GLenum data_type;
    bool   is_compressed;
    explicit NativePixelAttribs(GLenum PixelFormat = 0, GLenum DataType = 0, bool IsCompressed = false) :
        pixel_format(PixelFormat), data_type(DataType), is_compressed(IsCompressed)
    {}
};

inline uint32_t GetNumPixelFormatComponents(GLenum format)
{
    switch(format)
    {
        case GL_RGBA:
        case GL_RGBA_INTEGER:
            return 4;

        case GL_RGB:
        case GL_RGB_INTEGER:
            return 3;

        case GL_RG:
        case GL_RG_INTEGER:
            return 2;

        case GL_RED:
        case GL_RED_INTEGER:
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_STENCIL:
            return 1;

        default:
            UNEXPECTED("Unknonw pixel format");
            return 0;
    };
}

inline uint32_t GetPixelTypeSize(GLenum type)
{
    switch(type)
    {
        case GL_FLOAT:
            return sizeof(GLfloat);

        case GL_UNSIGNED_INT_10_10_10_2:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_10F_11F_11F_REV:
        case GL_UNSIGNED_INT_24_8:
        case GL_UNSIGNED_INT_5_9_9_9_REV:
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);

        case GL_INT:
            return sizeof(GLint);
        case GL_HALF_FLOAT:
            return sizeof(GLhalf);

        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_5_6_5_REV:
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_1_5_5_5_REV:
        case GL_UNSIGNED_SHORT:
            return sizeof(GLushort);

        case GL_SHORT:
            return sizeof(GLshort);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);
        case GL_BYTE:
            return sizeof(GLbyte);

        case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
            return sizeof(GLfloat) + sizeof(GLuint);

        default:
            UNEXPECTED("Unknonw pixel type");
            return 0;
    }
}

GLenum             TexFormatToGLInternalTexFormat(TEXTURE_FORMAT tex_format, uint32_t bind_flags = 0);
TEXTURE_FORMAT     GLInternalTexFormatToTexFormat(GLenum glformat);
GLenum             CorrectGLTexFormat(GLenum gltex_format, uint32_t bind_flags);
NativePixelAttribs GetNativePixelTransferAttribs(TEXTURE_FORMAT tex_format);
GLenum             AccessFlags2GLAccess(uint32_t uav_access_flags);
GLenum             TypeToGLTexFormat(VALUE_TYPE val_type, uint32_t num_components, bool is_normalized);
GLenum             StencilOp2GlStencilOp(STENCIL_OP stencil_op);
GLenum             BlendFactor2GLBlend(BLEND_FACTOR bf);
GLenum             BlendOperation2GLBlendOp(BLEND_OPERATION blend_op);
}   // namespace evnt
#endif   // TYPECONVERSIONS_H
