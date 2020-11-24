#include "typeconversions.h"
#include "../../core/exception.h"
#include "../../log/debug_messages.h"
#include <cassert>

namespace evnt
{
GLenum GetEnum(BufferOptions opt)
{
    GLenum qualifiers = GL_NONE;

    if(opt.map_read)
        qualifiers |= GL_MAP_READ_BIT;
    if(opt.map_write)
        qualifiers |= GL_MAP_WRITE_BIT;
    if(opt.map_persistent)
        qualifiers |= GL_MAP_PERSISTENT_BIT;
    if(opt.map_coherent)
        qualifiers |= GL_MAP_COHERENT_BIT;
    if(opt.dynamic_storage)
        qualifiers |= GL_DYNAMIC_STORAGE_BIT;
    if(opt.client_storage)
        qualifiers |= GL_CLIENT_STORAGE_BIT;

    return qualifiers;
}

GLenum GetEnum(MappingOptions opt)
{
    GLenum qualifiers = GL_NONE;

    if(opt.map_read)
        qualifiers |= GL_MAP_READ_BIT;
    if(opt.map_write)
        qualifiers |= GL_MAP_WRITE_BIT;
    if(opt.map_persistent)
        qualifiers |= GL_MAP_PERSISTENT_BIT;
    if(opt.map_coherent)
        qualifiers |= GL_MAP_COHERENT_BIT;
    if(opt.flush_explicit)
        qualifiers |= GL_MAP_FLUSH_EXPLICIT_BIT;
    if(opt.invalidate_range)
        qualifiers |= GL_MAP_INVALIDATE_RANGE_BIT;
    if(opt.invalidate_buffer)
        qualifiers |= GL_MAP_INVALIDATE_BUFFER_BIT;
    if(opt.unsynchronized)
        qualifiers |= GL_MAP_UNSYNCHRONIZED_BIT;

    return qualifiers;
}

GLenum StencilOpToGlStencilOp(StencilOperation stencil_op)
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

GLenum BlendFactorToGLBlend(BlendFactor bf)
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

GLenum BlendOperationToGLBlendOp(BlendOperation blend_op)
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

GLenum CompareFuncToGLCompareFunc(ComparisonFunc func)
{
    switch(func)
    {
        case ComparisonFunc::COMPARISON_FUNC_UNKNOWN:
            UNEXPECTED("Comparison function is not specified");
            return GL_ALWAYS;
        case ComparisonFunc::COMPARISON_FUNC_NEVER:
            return GL_NEVER;
        case ComparisonFunc::COMPARISON_FUNC_LESS:
            return GL_LESS;
        case ComparisonFunc::COMPARISON_FUNC_EQUAL:
            return GL_EQUAL;
        case ComparisonFunc::COMPARISON_FUNC_LESS_EQUAL:
            return GL_LEQUAL;
        case ComparisonFunc::COMPARISON_FUNC_GREATER:
            return GL_GREATER;
        case ComparisonFunc::COMPARISON_FUNC_NOT_EQUAL:
            return GL_NOTEQUAL;
        case ComparisonFunc::COMPARISON_FUNC_GREATER_EQUAL:
            return GL_GEQUAL;
        case ComparisonFunc::COMPARISON_FUNC_ALWAYS:
            return GL_ALWAYS;
        default:
            UNEXPECTED("Unknown comparison func");
            return GL_ALWAYS;
    }
}

GLenum WrapModeToGLWrapMode(WrapMode mode)
{
    switch(mode)
    {
        case WrapMode::TEXTURE_ADDRESS_UNKNOWN:
            UNEXPECTED("Texture address mode is not specified");
            return GL_CLAMP_TO_EDGE;
        case WrapMode::TEXTURE_ADDRESS_WRAP:
            return GL_REPEAT;
        case WrapMode::TEXTURE_ADDRESS_MIRROR:
            return GL_MIRRORED_REPEAT;
        case WrapMode::TEXTURE_ADDRESS_CLAMP:
            return GL_CLAMP_TO_EDGE;
        case WrapMode::TEXTURE_ADDRESS_BORDER:
            return GL_CLAMP_TO_BORDER;
        case WrapMode::TEXTURE_ADDRESS_MIRROR_ONCE:
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

GLenum MinFilterToGLMinFilter(MinificationFilter fl)
{
    switch(fl)
    {
        case MinificationFilter::Nearest:
            return GL_NEAREST;
        case MinificationFilter::Linear:
            return GL_LINEAR;
        case MinificationFilter::NearestMipmapNearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        case MinificationFilter::LinearMipmapNearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        case MinificationFilter::NearestMipmapLinear:
            return GL_NEAREST_MIPMAP_LINEAR;
        case MinificationFilter::LinearMipmapLinear:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            UNEXPECTED("Unknown MinificationFilter mode");
            return GL_NEAREST;
    }
}

GLenum MagFilterToGLMagFilter(MagnificationFilter fl)
{
    switch(fl)
    {
        case MagnificationFilter::Nearest:
            return GL_NEAREST;
        case MagnificationFilter::Linear:
            return GL_LINEAR;
        default:
            UNEXPECTED("Unknown MagnificationFilter mode");
            return GL_NEAREST;
    }
}

GLenum TexChannelToGLTexChannel(TextureChannel tex_chan)
{
    switch(tex_chan)
    {
        case TextureChannel::S:
            return GL_TEXTURE_WRAP_S;
        case TextureChannel::T:
            return GL_TEXTURE_WRAP_T;
        case TextureChannel::R:
            return GL_TEXTURE_WRAP_R;
        default:
            UNEXPECTED("Unknown TextureChannel mode");
            return GL_TEXTURE_WRAP_S;
    }
}

GLenum TextureInternalFormatToGLTextureInternalFormat(TextureInternalFormat fmt)
{
    switch(fmt)
    {
        case TextureInternalFormat::Depth16I:
            return GL_DEPTH_COMPONENT16;
        case TextureInternalFormat::Depth24I:
            return GL_DEPTH_COMPONENT24;
        case TextureInternalFormat::Depth32I:
            return GL_DEPTH_COMPONENT32;
        case TextureInternalFormat::Depth32F:
            return GL_DEPTH_COMPONENT32F;
        case TextureInternalFormat::Depth24IStencil8UI:
            return GL_DEPTH24_STENCIL8;
        case TextureInternalFormat::Depth32FStencil8UI:
            return GL_DEPTH32F_STENCIL8;
        case TextureInternalFormat::Stencil8UI:
            return GL_STENCIL_INDEX8;
        case TextureInternalFormat::R8:
            return GL_R8;
        case TextureInternalFormat::RG8:
            return GL_RG8;
        case TextureInternalFormat::RGB8:
            return GL_RGB8;
        case TextureInternalFormat::RGBA8:
            return GL_RGBA8;
        case TextureInternalFormat::R8I:
            return GL_R8I;
        case TextureInternalFormat::RG8I:
            return GL_RG8I;
        case TextureInternalFormat::RGB8I:
            return GL_RGB8I;
        case TextureInternalFormat::RGBA8I:
            return GL_RGBA8I;
        case TextureInternalFormat::R16I:
            return GL_R16I;
        case TextureInternalFormat::RG16I:
            return GL_RG16I;
        case TextureInternalFormat::RGB16I:
            return GL_RGB16I;
        case TextureInternalFormat::RGBA16I:
            return GL_RGBA16I;
        case TextureInternalFormat::R16F:
            return GL_R16F;
        case TextureInternalFormat::RG16F:
            return GL_RG16F;
        case TextureInternalFormat::RGB16F:
            return GL_RGB16F;
        case TextureInternalFormat::RGBA16F:
            return GL_RGBA16F;
        case TextureInternalFormat::R32F:
            return GL_R32F;
        case TextureInternalFormat::RG32F:
            return GL_RG32F;
        case TextureInternalFormat::RGB32F:
            return GL_RGB32F;
        case TextureInternalFormat::RGBA32F:
            return GL_RGBA32F;
        default:
            UNEXPECTED("Unknown TextureInternalFormat mode");
            return GL_NEAREST;
    }
}

GLenum TexturePixelFormatToGLTexturePixelFormat(TexturePixelFormat fmt)
{
    switch(fmt)
    {
        case TexturePixelFormat::R:
            return GL_RED;
        case TexturePixelFormat::RG:
            return GL_RG;
        case TexturePixelFormat::RGB:
            return GL_RGB;
        case TexturePixelFormat::BGR:
            return GL_BGR;
        case TexturePixelFormat::RGBA:
            return GL_RGBA;
        case TexturePixelFormat::BGRA:
            return GL_BGRA;
        case TexturePixelFormat::Depth:
            return GL_DEPTH_COMPONENT;
        case TexturePixelFormat::Stencil:
            return GL_STENCIL_INDEX;
        default:
            UNEXPECTED("Unknown TexturePixelFormat mode");
            return GL_NEAREST;
    }
}

GLenum TextureDataTypeToGLTextureDataType(TextureDataType dtype)
{
    switch(dtype)
    {
        case TextureDataType::UnsignedByte:
            return GL_UNSIGNED_BYTE;
        case TextureDataType::UnsignedInt:
            return GL_UNSIGNED_INT;
        case TextureDataType::int32_t:
            return GL_INT;
        case TextureDataType::Float:
            return GL_FLOAT;
        default:
            UNEXPECTED("Unknown TextureDataType mode");
            return GL_NEAREST;
    }
}

int32_t GetByteDepth(TexturePixelFormat format, TextureDataType type)
{
    int32_t base = 1;
    switch(type)
    {
        case TextureDataType::UnsignedByte:
            base *= 1;
            break;
        case TextureDataType::UnsignedInt:
            base *= 4;
            break;
        case TextureDataType::int32_t:
            base *= 4;
            break;
        case TextureDataType::Float:
            base *= 4;
            break;
    }

    switch(format)
    {
        case TexturePixelFormat::R:
            base *= 1;
            break;
        case TexturePixelFormat::RG:
            base *= 2;
            break;
        case TexturePixelFormat::RGB:
        case TexturePixelFormat::BGR:
            base *= 3;
            break;
        case TexturePixelFormat::RGBA:
        case TexturePixelFormat::BGRA:
            base *= 4;
            break;
        case TexturePixelFormat::Depth:
        case TexturePixelFormat::Stencil:
            UNEXPECTED("Unknown TexturePixelFormat byte depth for Depth OR Stencil");
            break;
    }
    return base;
}

GLenum PrimitiveTypeToGLPrimitiveType(PrimitiveType pt)
{
    switch(pt)
    {
        case PrimitiveType::Triangles:
            return GL_TRIANGLES;

        default:
            EV_EXCEPT("Unknown primitive type");
    }
}

GLbitfield MemoryBarrierTypeToGLMemoryBarrierType(MemoryBarrierType mb)
{
    switch(mb)
    {
        case MemoryBarrierType::VertexAttribArray:
            return GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
        case MemoryBarrierType::IndexBuffer:
            return GL_ELEMENT_ARRAY_BARRIER_BIT;
        case MemoryBarrierType::Uniform:
            return GL_UNIFORM_BARRIER_BIT;
        case MemoryBarrierType::TextureFetch:
            return GL_TEXTURE_FETCH_BARRIER_BIT;
        case MemoryBarrierType::ShaderImageAccess:
            return GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
        case MemoryBarrierType::CommandBarrier:
            return GL_COMMAND_BARRIER_BIT;
        case MemoryBarrierType::PixelBuffer:
            return GL_PIXEL_BUFFER_BARRIER_BIT;
        case MemoryBarrierType::TextureUpdate:
            return GL_TEXTURE_UPDATE_BARRIER_BIT;
        case MemoryBarrierType::BufferUpdate:
            return GL_BUFFER_UPDATE_BARRIER_BIT;
        case MemoryBarrierType::QueryBuffer:
            return GL_QUERY_BUFFER_BARRIER_BIT;
        case MemoryBarrierType::ClientMappedBuffer:
            return GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT;
        case MemoryBarrierType::Framebuffer:
            return GL_FRAMEBUFFER_BARRIER_BIT;
        case MemoryBarrierType::TransformFeedback:
            return GL_TRANSFORM_FEEDBACK_BARRIER_BIT;
        case MemoryBarrierType::AtomicCounter:
            return GL_ATOMIC_COUNTER_BARRIER_BIT;
        case MemoryBarrierType::ShaderStorageBuffer:
            return GL_SHADER_STORAGE_BARRIER_BIT;
        case MemoryBarrierType::All:
            return GL_ALL_BARRIER_BITS;
        default:
            EV_EXCEPT("Unknown MemoryBarrier type");
    }
}
}   // namespace evnt
