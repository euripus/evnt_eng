#ifndef ISHADERRESOURCEVARIABLE_H
#define ISHADERRESOURCEVARIABLE_H

#include "ideviceobject.h"
#include "ishader.h"

namespace evnt
{
/// Describes the type of the shader resource variable
enum SHADER_RESOURCE_VARIABLE_TYPE : uint8_t
{
    /// Shader resource bound to the variable is the same for all SRB instances.
    /// It must be set *once* directly through Pipeline State object.
    SHADER_RESOURCE_VARIABLE_TYPE_STATIC = 0,
    /// Shader resource bound to the variable is specific to the shader resource binding
    /// instance (see evnt::IShaderResourceBinding). It must be set *once* through
    /// evnt::IShaderResourceBinding interface. It cannot be set through evnt::IPipelineState
    /// interface and cannot be change once bound.
    SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE,
    /// Shader variable binding is dynamic. It can be set multiple times for every instance of shader resource
    /// binding (see evnt::IShaderResourceBinding). It cannot be set through evnt::IPipelineState
    /// interface.
    SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC,
    /// Total number of shader variable types
    SHADER_RESOURCE_VARIABLE_TYPE_NUM_TYPES
};

static_assert(SHADER_RESOURCE_VARIABLE_TYPE_STATIC == 0 && SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE == 1
                  && SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC == 2
                  && SHADER_RESOURCE_VARIABLE_TYPE_NUM_TYPES == 3,
              "BIND_SHADER_RESOURCES_UPDATE_* flags rely on shader variable SHADER_RESOURCE_VARIABLE_TYPE_* "
              "values being 0,1,2");

/// Shader resource binding flags
enum BIND_SHADER_RESOURCES_FLAGS : uint32_t
{
    /// Indicates that static shader variable bindings are to be updated.
    BIND_SHADER_RESOURCES_UPDATE_STATIC = (0x01 << SHADER_RESOURCE_VARIABLE_TYPE_STATIC),
    /// Indicates that mutable shader variable bindings are to be updated.
    BIND_SHADER_RESOURCES_UPDATE_MUTABLE = (0x01 << SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE),
    /// Indicates that dynamic shader variable bindings are to be updated.
    BIND_SHADER_RESOURCES_UPDATE_DYNAMIC = (0x01 << SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC),
    /// Indicates that all shader variable types (static, mutable and dynamic) are to be updated.
    /// \note If none of BIND_SHADER_RESOURCES_UPDATE_STATIC, BIND_SHADER_RESOURCES_UPDATE_MUTABLE,
    ///       and BIND_SHADER_RESOURCES_UPDATE_DYNAMIC flags are set, all variable types are updated
    ///       as if BIND_SHADER_RESOURCES_UPDATE_ALL was specified.
    BIND_SHADER_RESOURCES_UPDATE_ALL =
        (BIND_SHADER_RESOURCES_UPDATE_STATIC | BIND_SHADER_RESOURCES_UPDATE_MUTABLE
         | BIND_SHADER_RESOURCES_UPDATE_DYNAMIC),
    /// If this flag is specified, all existing bindings will be preserved and
    /// only unresolved ones will be updated.
    /// If this flag is not specified, every shader variable will be
    /// updated if the mapping contains corresponding resource.
    BIND_SHADER_RESOURCES_KEEP_EXISTING = 0x08,
    /// If this flag is specified, all shader bindings are expected
    /// to be resolved after the call. If this is not the case, debug message
    /// will be displayed.
    /// \note Only these variables are verified that are being updated by setting
    ///       BIND_SHADER_RESOURCES_UPDATE_STATIC, BIND_SHADER_RESOURCES_UPDATE_MUTABLE, and
    ///       BIND_SHADER_RESOURCES_UPDATE_DYNAMIC flags.
    BIND_SHADER_RESOURCES_VERIFY_ALL_RESOLVED = 0x10
};

/// Shader resource variable
class IShaderResourceVariable
{
public:
    /// Binds resource to the variable
    /// \remark The method performs run-time correctness checks.
    ///         For instance, shader resource view cannot
    ///         be assigned to a constant buffer variable.
    virtual void set(IDeviceObject * pObject) = 0;

    /// Binds resource array to the variable
    /// \param [in] ppObjects    - pointer to the array of objects
    /// \param [in] FirstElement - first array element to set
    /// \param [in] NumElements  - number of objects in ppObjects array
    /// \remark The method performs run-time correctness checks.
    ///         For instance, shader resource view cannot
    ///         be assigned to a constant buffer variable.
    virtual void setArray(IDeviceObject * const * ppObjects, uint32_t FirstElement, uint32_t NumElements) = 0;

    /// Returns the shader resource variable type
    virtual SHADER_RESOURCE_VARIABLE_TYPE GetType() const = 0;

    /// Returns shader resource description. See evnt::ShaderResourceDesc.
    virtual ShaderResourceDesc getResourceDesc() const = 0;

    /// Returns the variable index that can be used to access the variable.
    virtual uint32_t getIndex() const = 0;
};
}   // namespace evnt

#endif   // ISHADERRESOURCEVARIABLE_H
