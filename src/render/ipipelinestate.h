#ifndef IPIPELINESTATE_H
#define IPIPELINESTATE_H

#include "blend.h"
#include "depthstencilstate.h"
#include "ideviceobject.h"
#include "inputlayout.h"
#include "isampler.h"
#include "ishader.h"
#include "ishaderresourcebinding.h"
#include "ishaderresourcevariable.h"
#include "rasterizerstate.h"

namespace evnt
{
/// Sample description
/// This structure is used by GraphicsPipelineDesc to describe multisampling parameters
struct SampleDesc
{
    /// Sample count
    int8_t count = 1;
    /// Quality
    int8_t quality = 0;

    SampleDesc() noexcept {}
    SampleDesc(int8_t _Count, int8_t _Quality) noexcept : count(_Count), quality(_Quality) {}
};

/// Describes shader variable
struct ShaderResourceVariableDesc
{
    /// Shader stages this resources variable applies to. More than one shader stage can be specified.
    SHADER_TYPE shader_stages = SHADER_TYPE_UNKNOWN;
    /// Shader variable name
    std::string name = nullptr;
    /// Shader variable type. See Diligent::SHADER_RESOURCE_VARIABLE_TYPE for a list of allowed types
    SHADER_RESOURCE_VARIABLE_TYPE Type = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    ShaderResourceVariableDesc() noexcept {}
    ShaderResourceVariableDesc(SHADER_TYPE _ShaderStages, const char * _Name,
                               SHADER_RESOURCE_VARIABLE_TYPE _Type) noexcept :
        shader_stages(_ShaderStages), name(_Name), Type(_Type)
    {}
};

/// Static sampler description
struct StaticSamplerDesc
{
    /// Shader stages that this static sampler applies to. More than one shader stage can be specified.
    SHADER_TYPE shader_stages = SHADER_TYPE_UNKNOWN;
    /// The name of the sampler itself or the name of the texture variable that
    /// this static sampler is assigned to if combined texture samplers are used.
    std::string sampler_or_texture_name = nullptr;
    /// Sampler description
    SamplerDesc desc;

    StaticSamplerDesc() noexcept {}
    StaticSamplerDesc(SHADER_TYPE _ShaderStages, const char * _SamplerOrTextureName,
                      const SamplerDesc & _Desc) noexcept :
        shader_stages(_ShaderStages), sampler_or_texture_name(_SamplerOrTextureName), desc(_Desc)
    {}
};

/// Pipeline layout description
struct PipelineResourceLayoutDesc
{
    /// Default shader resource variable type. This type will be used if shader
    /// variable description is not found in the Variables array
    /// or if Variables == nullptr
    SHADER_RESOURCE_VARIABLE_TYPE default_variable_type = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    /// Number of elements in Variables array
    uint32_t num_variables = 0;
    /// Array of shader resource variable descriptions
    const ShaderResourceVariableDesc * Variables = nullptr;
    /// Number of static samplers in StaticSamplers array
    uint32_t num_static_samplers = 0;
    /// Array of static sampler descriptions
    const StaticSamplerDesc * static_samplers = nullptr;
};

/// Graphics pipeline state description
/// This structure describes the graphics pipeline state and is part of the PipelineStateDesc structure.
struct GraphicsPipelineDesc
{
    /// Vertex shader to be used with the pipeline
    IShader * p_vs = nullptr;
    /// Pixel shader to be used with the pipeline
    IShader * p_ps = nullptr;
    /// Domain shader to be used with the pipeline
    IShader * p_ds = nullptr;
    /// Hull shader to be used with the pipeline
    IShader * p_hs = nullptr;
    /// Geometry shader to be used with the pipeline
    IShader * p_gs = nullptr;
    // D3D12_STREAM_OUTPUT_DESC StreamOutput;
    /// Blend state description
    BlendStateDesc blend_desc;
    /// 32-bit sample mask that determines which samples get updated
    /// in all the active render targets. A sample mask is always applied;
    /// it is independent of whether multisampling is enabled, and does not
    /// depend on whether an application uses multisample render targets.
    uint32_t sample_mask = 0xFFFFFFFF;
    /// Rasterizer state description
    RasterizerStateDesc rasterizer_desc;
    /// Depth-stencil state description
    DepthStencilStateDesc depth_stencil_desc;
    /// Input layout
    InputLayoutDesc input_layout;
    // D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue;
    /// Primitive topology type
    PRIMITIVE_TOPOLOGY primitive_topology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    /// Number of viewports used by this pipeline
    int8_t num_viewports = 1;
    /// Number of render targets in the RTVFormats member
    int8_t num_render_targets = 0;
    /// Render target formats
    TEXTURE_FORMAT RTV_formats[8] = {};
    /// Depth-stencil format
    TEXTURE_FORMAT DSV_format = TEX_FORMAT_UNKNOWN;
    /// Multisampling parameters
    SampleDesc smpl_desc;
    /// Node mask.
    uint32_t node_mask = 0;

    // D3D12_CACHED_PIPELINE_STATE CachedPSO;
    // D3D12_PIPELINE_STATE_FLAGS Flags;
};

/// Compute pipeline state description
/// This structure describes the compute pipeline state and is part of the PipelineStateDesc structure.
struct ComputePipelineDesc
{
    /// Compute shader to be used with the pipeline
    IShader * p_cs = nullptr;
};

/// Pipeline state description
struct PipelineStateDesc : DeviceObjectAttribs
{
    /// Flag indicating if pipeline state is a compute pipeline state
    bool is_compute_pipeline = false;
    /// Shader resource binding allocation granularity
    /// This member defines allocation granularity for internal resources required by the shader resource
    /// binding object instances.
    uint32_t SRB_allocation_granularity = 1;
    /// Defines which command queues this pipeline state can be used with
    uint64_t command_queue_mask = 1;
    /// Pipeline layout description
    PipelineResourceLayoutDesc resource_layout;
    /// Graphics pipeline state description. This memeber is ignored if IsComputePipeline == True
    GraphicsPipelineDesc graphics_pipeline;
    /// Compute pipeline state description. This memeber is ignored if IsComputePipeline == False
    ComputePipelineDesc compute_pipeline;
};

/**
 * Pipeline state interface
 */
class IPipelineState : public IDeviceObject
{
public:
    /// Returns the blend state description used to create the object
    const PipelineStateDesc & getDesc() const override = 0;

    /// Binds resources for all shaders in the pipeline state

    /// \param [in] ShaderFlags - Flags that specify shader stages, for which resources will be bound.
    ///                           Any combination of Diligent::SHADER_TYPE may be used.
    /// \param [in] pResourceMapping - Pointer to the resource mapping interface.
    /// \param [in] Flags - Additional flags. See Diligent::BIND_SHADER_RESOURCES_FLAGS.
    virtual void bindStaticResources(uint32_t ShaderFlags, IResourceMapping * pResourceMapping,
                                     uint32_t Flags) = 0;

    /// Returns the number of static shader resource variables.

    /// \param [in] ShaderType - Type of the shader.
    /// \remark Only static variables (that can be accessed directly through the PSO) are counted.
    ///         Mutable and dynamic variables are accessed through Shader Resource Binding object.
    virtual uint32_t getStaticVariableCount(SHADER_TYPE ShaderType) const = 0;

    /// Returns static shader resource variable. If the variable is not found,
    /// returns nullptr.

    /// \param [in] ShaderType - Type of the shader to look up the variable.
    ///                          Must be one of Diligent::SHADER_TYPE.
    /// \param [in] Name - Name of the variable.
    /// \remark The method does not increment the reference counter
    ///         of the returned interface.
    virtual IShaderResourceVariable * getStaticShaderVariable(SHADER_TYPE ShaderType, const char * Name) = 0;

    /// Returns static shader resource variable by its index.

    /// \param [in] ShaderType - Type of the shader to look up the variable.
    ///                          Must be one of Diligent::SHADER_TYPE.
    /// \param [in] Index - Shader variable index. The index must be between
    ///                     0 and the total number of variables returned by
    ///                     GetStaticVariableCount().
    /// \remark Only static shader resource variables can be accessed through this method.
    ///         Mutable and dynamic variables are accessed through Shader Resource
    ///         Binding object
    virtual IShaderResourceVariable * getStaticShaderVariable(SHADER_TYPE ShaderType, uint32_t Index) = 0;

    /// Creates a shader resource binding object

    /// \param [out] ppShaderResourceBinding - memory location where pointer to the new shader resource
    ///                                        binding object is written.
    /// \param [in] InitStaticResources      - if set to true, the method will initialize static resources in
    ///                                        the created object, which has the exact same effect as calling
    ///                                        IShaderResourceBinding::InitializeStaticResources().
    virtual void createShaderResourceBinding(IShaderResourceBinding ** ppShaderResourceBinding,
                                             bool                      InitStaticResources = false) = 0;

    /// Checks if this pipeline state object is compatible with another PSO

    /// If two pipeline state objects are compatible, they can use shader resource binding
    /// objects interchangebly, i.e. SRBs created by one PSO can be committed
    /// when another PSO is bound.
    /// \param [in] pPSO - Pointer to the pipeline state object to check compatibility with
    /// \return     true if this PSO is compatbile with pPSO. false otherwise.
    /// \remarks    The function only checks that shader resource layouts are compatible, but
    ///             does not check if resource types match. For instance, if a pixel shader in one PSO
    ///             uses a texture at slot 0, and a pixel shader in another PSO uses texture array at slot 0,
    ///             the pipelines will be compatible. However, if you try to use SRB object from the first
    ///             pipeline to commit resources for the second pipeline, a runtime error will occur.\n The
    ///             function only checks compatibility of shader resource layouts. It does not take into
    ///             account vertex shader input layout, number of outputs, etc.
    virtual bool isCompatibleWith(const IPipelineState * pPSO) const = 0;
};
}   // namespace evnt

#endif   // IPIPELINESTATE_H
