#ifndef GRAPHICSACCESSORIES_H
#define GRAPHICSACCESSORIES_H

#include "../graphics_types.h"
#include "../ibufferview.h"
#include "../ishaderresourcevariable.h"
#include "../itexture.h"
#include <cassert>
#include <iterator>
#include <string>

namespace evnt
{
/// Template structure to convert VALUE_TYPE enumeration into C-type
template<VALUE_TYPE ValType>
struct VALUE_TYPE2CType
{};

/// VALUE_TYPE2CType<> template specialization for 8-bit integer value type.

/// Usage example:
///
///     VALUE_TYPE2CType<VT_INT8>::CType MyInt8Var;
template<>
struct VALUE_TYPE2CType<VT_INT8>
{
    using CType = int8_t;
};

/// VALUE_TYPE2CType<> template specialization for 16-bit integer value type.

/// Usage example:
///
///     VALUE_TYPE2CType<VT_INT16>::CType MyInt16Var;
template<>
struct VALUE_TYPE2CType<VT_INT16>
{
    using CType = int16_t;
};

/// VALUE_TYPE2CType<> template specialization for 32-bit integer value type.

/// Usage example:
///
///     VALUE_TYPE2CType<VT_INT32>::CType MyInt32Var;
template<>
struct VALUE_TYPE2CType<VT_INT32>
{
    using CType = int32_t;
};

/// VALUE_TYPE2CType<> template specialization for 8-bit unsigned-integer value type.

/// Usage example:
///
///     VALUE_TYPE2CType<VT_UINT8>::CType MyUint8Var;
template<>
struct VALUE_TYPE2CType<VT_UINT8>
{
    using CType = uint8_t;
};

/// VALUE_TYPE2CType<> template specialization for 16-bit unsigned-integer value type.

/// Usage example:
///
///     VALUE_TYPE2CType<VT_UINT16>::CType MyUint16Var;
template<>
struct VALUE_TYPE2CType<VT_UINT16>
{
    using CType = uint16_t;
};

/// VALUE_TYPE2CType<> template specialization for 32-bit unsigned-integer value type.

/// Usage example:
///
///     VALUE_TYPE2CType<VT_uint32_t>::CType Myuint32_tVar;
template<>
struct VALUE_TYPE2CType<VT_UINT32>
{
    using CType = uint32_t;
};

/// VALUE_TYPE2CType<> template specialization for half-precision 16-bit floating-point value type.

/// Usage example:
///
///     VALUE_TYPE2CType<VT_FLOAT16>::CType MyFloat16Var;
///
/// \note 16-bit floating-point values have no corresponding C++ type and are translated to Uint16
template<>
struct VALUE_TYPE2CType<VT_FLOAT16>
{
    using CType = uint16_t;
};

/// VALUE_TYPE2CType<> template specialization for full-precision 32-bit floating-point value type.

/// Usage example:
///
///     VALUE_TYPE2CType<VT_FLOAT32>::CType MyFloat32Var;
template<>
struct VALUE_TYPE2CType<VT_FLOAT32>
{
    using CType = float;
};

static const uint32_t ValueTypeToSizeMap[] = {0,
                                              sizeof(VALUE_TYPE2CType<VT_INT8>::CType),
                                              sizeof(VALUE_TYPE2CType<VT_INT16>::CType),
                                              sizeof(VALUE_TYPE2CType<VT_INT32>::CType),
                                              sizeof(VALUE_TYPE2CType<VT_UINT8>::CType),
                                              sizeof(VALUE_TYPE2CType<VT_UINT16>::CType),
                                              sizeof(VALUE_TYPE2CType<VT_UINT32>::CType),
                                              sizeof(VALUE_TYPE2CType<VT_FLOAT16>::CType),
                                              sizeof(VALUE_TYPE2CType<VT_FLOAT32>::CType)};
static_assert(VT_NUM_TYPES == VT_FLOAT32 + 1, "Not all value type sizes initialized.");

/// Returns the size of the specified value type
inline uint32_t GetValueSize(VALUE_TYPE val)
{
    assert(val < std::size(ValueTypeToSizeMap));
    return ValueTypeToSizeMap[val];
}

/// Returns the string representing the specified value type
const char * GetValueTypeString(VALUE_TYPE val);

/// Returns invariant texture format attributes, see TextureFormatInfo for details.
/// \param [in] format - Texture format which attributes are requested for.
/// \return Constant reference to the TextureFormatAttribs structure containing
///         format attributes.
const TextureFormatInfo & GetTextureFormatInfo(TEXTURE_FORMAT format);

/// Returns the default format for a specified texture view type
/// The default view is defined as follows:
/// * For a fully qualified texture format, the SRV/RTV/UAV view format is the same as texture format;
///   DSV format, if avaialble, is adjusted accrodingly (R32_FLOAT -> D32_FLOAT)
/// * For 32-bit typeless formats, default view is XXXX32_FLOAT (where XXXX are the actual format
/// components)\n
/// * For 16-bit typeless formats, default view is XXXX16_FLOAT (where XXXX are the actual format
/// components)\n
/// ** R16_TYPELESS is special. If BIND_DEPTH_STENCIL flag is set, it is translated to R16_UNORM/D16_UNORM;
///    otherwise it is translated to R16_FLOAT.
/// * For 8-bit typeless formats, default view is XXXX8_UNORM (where XXXX are the actual format components)\n
/// * sRGB is always chosen if it is available (RGBA8_UNORM_SRGB, TEX_FORMAT_BC1_UNORM_SRGB, etc.)
/// * For combined depth-stencil formats, SRV format references depth component (R24_UNORM_X8_TYPELESS for
/// D24S8 formats, and
///   R32_FLOAT_X8X24_TYPELESS for D32S8X24 formats)
/// * For compressed formats, only SRV format is defined
///
/// \param [in] texture_format - texture format, for which the view format is requested
/// \param [in] view_type - texture view type
/// \param [in] bind_flags - texture bind flags
/// \return  texture view type format
TEXTURE_FORMAT GetDefaultTextureViewFormat(TEXTURE_FORMAT texture_format, TEXTURE_VIEW_TYPE view_type,
                                           uint32_t bind_flags);

/// Returns the default format for a specified texture view type
/// \param [in] tex_desc - texture description
/// \param [in] view_type - texture view type
/// \return  texture view type format
inline TEXTURE_FORMAT GetDefaultTextureViewFormat(const TextureDesc & tex_desc, TEXTURE_VIEW_TYPE view_type)
{
    return GetDefaultTextureViewFormat(tex_desc.format, view_type, tex_desc.bind_flags);
}

/// Returns the literal name of a texture view type. For instance,
/// for a shader resource view, "TEXTURE_VIEW_SHADER_RESOURCE" will be returned.
/// \param [in] view_type - Texture view type.
/// \return Literal name of the texture view type.
const char * GetTexViewTypeLiteralName(TEXTURE_VIEW_TYPE view_type);

/// Returns the literal name of a buffer view type. For instance,
/// for an unordered access view, "BUFFER_VIEW_UNORDERED_ACCESS" will be returned.
/// \param [in] view_type - Buffer view type.
/// \return Literal name of the buffer view type.
const char * GetBufferViewTypeLiteralName(BUFFER_VIEW_TYPE view_type);

/// Returns the literal name of a shader type. For instance,
/// for a pixel shader, "SHADER_TYPE_PIXEL" will be returned.
/// \param [in] shader_type - Shader type.
/// \return Literal name of the shader type.
const char * GetShaderTypeLiteralName(SHADER_TYPE shader_type);

/// \param [in] shader_stages - Shader stages.
/// \return The string representing the shader stages. For example,
///         if shader_stages == SHADER_TYPE_VERTEX | SHADER_TYPE_PIXEL,
///         the following string will be returned:
///         "SHADER_TYPE_VERTEX, SHADER_TYPE_PIXEL"
std::string GetShaderStagesString(SHADER_TYPE shader_stages);

/// Returns the literal name of a shader variable type. For instance,
/// for SHADER_RESOURCE_VARIABLE_TYPE_STATIC, if get_full_name == true, "SHADER_RESOURCE_VARIABLE_TYPE_STATIC"
/// will be returned; if get_full_name == false, "static" will be returned
/// \param [in] var_type - Variable type.
/// \param [in] get_full_name - Whether to return string representation of the enum value
/// \return Literal name of the shader variable type.
const char * GetShaderVariableTypeLiteralName(SHADER_RESOURCE_VARIABLE_TYPE var_type,
                                              bool                          get_full_name = false);

/// Returns the literal name of a shader resource type. For instance,
/// for SHADER_RESOURCE_TYPE_CONSTANT_BUFFER, if get_full_name == true, "SHADER_RESOURCE_TYPE_CONSTANT_BUFFER"
/// will be returned; if get_full_name == false, "constant buffer" will be returned
/// \param [in] resource_type - Resource type.
/// \param [in] get_full_name - Whether to return string representation of the enum value
/// \return Literal name of the shader resource type.
const char * GetShaderResourceTypeLiteralName(SHADER_RESOURCE_TYPE resource_type, bool get_full_name = false);

/// Overloaded function that returns the literal name of a texture view type.
/// see GetTexViewTypeLiteralName().
inline const char * GetViewTypeLiteralName(TEXTURE_VIEW_TYPE tex_view_type)
{
    return GetTexViewTypeLiteralName(tex_view_type);
}

/// Overloaded function that returns the literal name of a buffer view type.
/// see GetBufferViewTypeLiteralName().
inline const char * GetViewTypeLiteralName(BUFFER_VIEW_TYPE buff_view_type)
{
    return GetBufferViewTypeLiteralName(buff_view_type);
}

/// Returns the string containing the map type
const char * GetMapTypeString(MAP_TYPE map_type);
/// Returns the string containing the usage
const char * GetUsageString(USAGE usage);
/// Returns the string containing the texture type
const char * GetResourceDimString(RESOURCE_DIMENSION tex_type);
/// Returns the string containing single bind flag
const char * GetBindFlagString(uint32_t bind_flag);
/// Returns the string containing the bind flags
std::string GetBindFlagsString(uint32_t bind_flags);
/// Returns the string containing the CPU access flags
std::string GetCPUAccessFlagsString(uint32_t cpu_access_flags);
/// Returns the string containing the texture description
std::string GetTextureDescString(const TextureDesc & desc);
/// Returns the string containing the buffer format description
std::string GetBufferFormatString(const BufferFormat & fmt);
/// Returns the string containing the buffer mode description
const char * GetBufferModeString(BUFFER_MODE mode);
/// Returns the string containing the buffer description
std::string GetBufferDescString(const BufferDesc & desc);
/// Returns the string containing the buffer mode description
const char * GetResourceStateFlagString(RESOURCE_STATE state);
std::string  GetResourceStateString(RESOURCE_STATE state);

/// Helper template function that converts object description into a string
template<typename TObjectDescType>
std::string GetObjectDescString(const TObjectDescType &)
{
    return "";
}

/// Template specialization for texture description
template<>
inline std::string GetObjectDescString(const TextureDesc & tex_desc)
{
    std::string Str("Tex desc: ");
    Str += GetTextureDescString(tex_desc);
    return Str;
}

/// Template specialization for buffer description
template<>
inline std::string GetObjectDescString(const BufferDesc & buff_desc)
{
    std::string Str("Buff desc: ");
    Str += GetBufferDescString(buff_desc);
    return Str;
}

inline bool IsComparisonFilter(FILTER_TYPE filter_type)
{
    return filter_type == FILTER_TYPE_COMPARISON_POINT || filter_type == FILTER_TYPE_COMPARISON_LINEAR
           || filter_type == FILTER_TYPE_COMPARISON_ANISOTROPIC;
}

inline bool IsAnisotropicFilter(FILTER_TYPE filter_type)
{
    return filter_type == FILTER_TYPE_ANISOTROPIC || filter_type == FILTER_TYPE_COMPARISON_ANISOTROPIC
           || filter_type == FILTER_TYPE_MINIMUM_ANISOTROPIC
           || filter_type == FILTER_TYPE_MAXIMUM_ANISOTROPIC;
}

/// Describes the mip level properties
struct MipLevelProperties
{
    uint32_t logical_width    = 0;
    uint32_t logical_height   = 0;
    uint32_t storage_width    = 0;
    uint32_t storage_height   = 0;
    uint32_t depth            = 1;
    uint32_t row_size         = 0;
    uint32_t depth_slice_size = 0;
    uint32_t mip_size         = 0;
};

uint32_t           ComputeMipLevelsCount(uint32_t width);
uint32_t           ComputeMipLevelsCount(uint32_t width, uint32_t height);
uint32_t           ComputeMipLevelsCount(uint32_t width, uint32_t height, uint32_t depth);
MipLevelProperties GetMipLevelProperties(const TextureDesc & tex_desc, uint32_t mip_level);

bool VerifyResourceStates(RESOURCE_STATE state, bool is_texture);

}   // namespace evnt

#endif   // GRAPHICSACCESSORIES_H
