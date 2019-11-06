#ifndef ISHADER_H
#define ISHADER_H

#include "ideviceobject.h"

namespace evnt
{
class IShaderSourceInputStreamFactory;
class IDataBlob;

/// Describes the shader type
enum SHADER_TYPE : uint32_t
{
    SHADER_TYPE_UNKNOWN  = 0x000,   ///< Unknown shader type
    SHADER_TYPE_VERTEX   = 0x001,   ///< Vertex shader
    SHADER_TYPE_PIXEL    = 0x002,   ///< Pixel (fragment) shader
    SHADER_TYPE_GEOMETRY = 0x004,   ///< Geometry shader
    SHADER_TYPE_HULL     = 0x008,   ///< Hull (tessellation control) shader
    SHADER_TYPE_DOMAIN   = 0x010,   ///< Domain (tessellation evaluation) shader
    SHADER_TYPE_COMPUTE  = 0x020    ///< Compute shader
};

enum SHADER_PROFILE : uint8_t
{
    SHADER_PROFILE_DEFAULT = 0,
    SHADER_PROFILE_DX_4_0,
    SHADER_PROFILE_DX_5_0,
    SHADER_PROFILE_DX_5_1,
    SHADER_PROFILE_GL_4_2
};

/// Describes shader source code language
enum SHADER_SOURCE_LANGUAGE : uint32_t
{
    /// Default language (GLSL for OpenGL/OpenGLES devices, HLSL for Direct3D11/Direct3D12 devices)
    SHADER_SOURCE_LANGUAGE_DEFAULT = 0,
    /// The source language is HLSL
    SHADER_SOURCE_LANGUAGE_HLSL,
    /// The source language is GLSL
    SHADER_SOURCE_LANGUAGE_GLSL
};

/// Shader description
struct ShaderDesc : DeviceObjectAttribs
{
    /// Shader type. See evnt::SHADER_TYPE.
    SHADER_TYPE    shader_type    = SHADER_TYPE_VERTEX;
    SHADER_PROFILE target_profile = SHADER_PROFILE_DEFAULT;
};

/// Shader source stream factory interface
/*class IShaderSourceInputStreamFactory
{
public:
    virtual void CreateInputStream(const evnt::char * Name, IFileStream ** ppStream) = 0;
};*/

struct ShaderMacro
{
    const char * name       = nullptr;
    const char * definition = nullptr;

    ShaderMacro() noexcept {}
    ShaderMacro(const char * _Name, const char * _Def) noexcept : name(_Name), definition(_Def) {}
};

/// Shader creation attributes
struct ShaderCreateInfo
{
    /// Source file path
    /// If source file path is provided, Source and ByteCode members must be null
    const char * file_path = nullptr;
    /// Pointer to the shader source input stream factory.
    /// The factory is used to load the shader source file if FilePath is not null.
    /// It is also used to create additional input streams for shader include files
    IShaderSourceInputStreamFactory * shader_source_stream_factory = nullptr;
    /// HLSL->GLSL conversion stream
    /// If HLSL->GLSL converter is used to convert HLSL shader source to
    /// GLSL, this member can provide pointer to the conversion stream. It is useful
    /// when the same file is used to create a number of different shaders. If
    /// ppConversionStream is null, the converter will parse the same file
    /// every time new shader is converted. If ppConversionStream is not null,
    /// the converter will write pointer to the conversion stream to *ppConversionStream
    /// the first time and will use it in all subsequent times.
    /// For all subsequent conversions, FilePath member must be the same, or
    /// new stream will be crated and warning message will be displayed.
    class IHLSL2GLSLConversionStream ** conversion_stream = nullptr;
    /// Shader source
    /// If shader source is provided, FilePath and ByteCode members must be null
    const char * source = nullptr;
    /// Compiled shader bytecode.
    /// If shader byte code is provided, FilePath and Source members must be null
    /// \note. This option is supported for D3D11, D3D12 and Vulkan backends.
    ///        For D3D11 and D3D12 backends, HLSL bytecode should be provided. Vulkan
    ///        backend expects SPIRV bytecode.
    ///        The bytecode must contain reflection information. If shaders were compiled
    ///        using fxc, make sure that /Qstrip_reflect option is *not* specified.
    ///        HLSL shaders need to be compiled against 4.0 profile or higher.
    const void * byte_code = nullptr;
    /// Size of the compiled shader bytecode
    /// Byte code size (in bytes) must be provided if ByteCode is not null
    size_t byte_code_size = 0;
    /// Shader entry point
    /// This member is ignored if ByteCode is not null
    const char * entry_point = "main";
    /// Shader macros
    /// This member is ignored if ByteCode is not null
    const ShaderMacro * macros = nullptr;
    /// If set to true, textures will be combined with texture samplers.
    /// The CombinedSamplerSuffix member defines the suffix added to the texture variable
    /// name to get corresponding sampler name. When using combined samplers,
    /// the sampler assigned to the shader resource view is automatically set when
    /// the view is bound. Otherwise samplers need to be explicitly set similar to other
    /// shader variables.
    bool use_combined_texture_samplers = false;
    /// If UseCombinedTextureSamplers is true, defines the suffix added to the
    /// texture variable name to get corresponding sampler name.  For example,
    /// for default value "_sampler", a texture named "tex" will be combined
    /// with sampler named "tex_sampler".
    /// If UseCombinedTextureSamplers is false, this member is ignored.
    const char * combined_sampler_suffix = "_sampler";
    /// Shader description. See evnt::ShaderDesc.
    ShaderDesc desc;
    /// Shader source language. See evnt::SHADER_SOURCE_LANGUAGE.
    SHADER_SOURCE_LANGUAGE source_language = SHADER_SOURCE_LANGUAGE_DEFAULT;
    /// Memory address where pointer to the compiler messages data blob will be written
    /// The buffer contains two null-terminated strings. The first one is the compiler
    /// output message. The second one is the full shader source code including definitions added
    /// by the engine. Data blob object must be released by the client.
    IDataBlob ** compiler_output = nullptr;
};

/// Describes shader resource type
enum SHADER_RESOURCE_TYPE : uint8_t
{
    /// Shader resource type is unknown
    SHADER_RESOURCE_TYPE_UNKNOWN = 0,
    /// Constant (uniform) buffer
    SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
    /// Shader resource view of a texture (sampled image)
    SHADER_RESOURCE_TYPE_TEXTURE_SRV,
    /// Shader resource view of a buffer (read-only storage image)
    SHADER_RESOURCE_TYPE_BUFFER_SRV,
    /// Unordered access view of a texture (sotrage image)
    SHADER_RESOURCE_TYPE_TEXTURE_UAV,
    /// Unordered access view of a buffer (storage buffer)
    SHADER_RESOURCE_TYPE_BUFFER_UAV,
    /// Sampler (separate sampler)
    SHADER_RESOURCE_TYPE_SAMPLER
};

/// Shader resource description
struct ShaderResourceDesc : public DeviceObjectAttribs
{
    /// Shader resource name
    std::string shader_name = {};
    /// Shader resource type, see evnt::SHADER_RESOURCE_TYPE.
    SHADER_RESOURCE_TYPE type = SHADER_RESOURCE_TYPE_UNKNOWN;
    /// Array size. For non-array resource this value is 1.
    uint32_t array_size = 0;
};

/// Shader interface
class IShader : public IDeviceObject
{
public:
    /// Returns the shader description
    const ShaderDesc & getDesc() const override = 0;
    /// Returns the total number of shader resources
    virtual uint32_t GetResourceCount() const = 0;
    /// Returns the pointer to the array of shader resources
    virtual ShaderResourceDesc GetResource(uint32_t Index) const = 0;
};
}   // namespace evnt

#endif   // ISHADER_H
