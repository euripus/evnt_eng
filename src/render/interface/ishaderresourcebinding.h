#ifndef ISHADERRESOURCEBINDING_H
#define ISHADERRESOURCEBINDING_H

#include "iresourcemapping.h"
#include "ishader.h"
#include "ishaderresourcevariable.h"

namespace evnt
{
class IPipelineState;

/// Shader resource binding interface
class IShaderResourceBinding
{
public:
    /// Returns pointer to the referenced buffer object.
    /// The method calls AddRef() on the returned interface,
    /// so Release() must be called to avoid memory leaks.
    virtual IPipelineState * getPipelineState() = 0;

    /// Binds mutable and dynamice resources using the resource mapping
    /// \param [in] ShaderFlags - Flags that specify shader stages, for which resources will be bound.
    ///                           Any combination of evnt::SHADER_TYPE may be used.
    /// \param [in] pResMapping - Shader resource mapping, where required resources will be looked up
    /// \param [in] Flags       - Additional flags. See evnt::BIND_SHADER_RESOURCES_FLAGS.
    virtual void bindResources(uint32_t ShaderFlags, IResourceMapping * pResMapping, uint32_t Flags) = 0;

    /// Returns variable
    /// \param [in] ShaderType - Type of the shader to look up the variable.
    ///                          Must be one of evnt::SHADER_TYPE.
    /// \param [in] Name       - Variable name
    virtual IShaderResourceVariable * getVariable(SHADER_TYPE ShaderType, const char * Name) = 0;

    /// Returns the total variable count for the specific shader stage.
    /// \param [in] ShaderType - Type of the shader.
    /// \remark The method only counts mutable and dynamic variables that can be accessed through
    ///         the Shader Resource Binding object. Static variables are accessed through the Shader
    ///         object.
    virtual uint32_t getVariableCount(SHADER_TYPE ShaderType) const = 0;

    /// Returns variable
    /// \param [in] ShaderType - Type of the shader to look up the variable.
    ///                          Must be one of evnt::SHADER_TYPE.
    /// \param [in] Index      - Variable index. The index must be between 0 and the total number
    ///                          of variables in this shader stage as returned by
    ///                          IShaderResourceBinding::GetVariableCount().
    /// \remark Only mutable and dynamic variables can be accessed through this method.
    ///         Static variables are accessed through the Shader object.
    virtual IShaderResourceVariable * getVariable(SHADER_TYPE ShaderType, uint32_t Index) = 0;

    /// Initializes static resources
    /// If the parent pipeline state object contain static resources
    /// (see evnt::SHADER_RESOURCE_VARIABLE_TYPE_STATIC), this method must be called
    /// once to initialize static resources in this shader resource binding object.
    /// The method must be called after all static variables are initialized
    /// in the PSO.
    /// \param [in] pPipelineState - Pipeline state to copy static shader resource
    ///                              bindings from. The pipeline state must be compatible
    ///                              with this shader resource binding object.
    ///                              If null pointer is provided, the pipeline state
    ///                              that this SRB object was created from is used.
    /// \note The method must be called exactly once. If static resources have
    ///       already been initialized and the method is called again, it will have
    ///       no effect and a warning messge will be displayed.
    virtual void initializeStaticResources(const IPipelineState * pPipelineState = nullptr) = 0;
};
}   // namespace evnt

#endif   // ISHADERRESOURCEBINDING_H
