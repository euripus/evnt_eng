#ifndef DEVICECAPS_H
#define DEVICECAPS_H

#include "graphics_types.h"

namespace evnt
{
/// Device type
enum class DeviceType : int32_t
{
    Undefined = 0,   ///< Undefined device
    D3D11,           ///< D3D11 device
    D3D12,           ///< D3D12 device
    OpenGL,          ///< OpenGL device
    OpenGLES,        ///< OpenGLES device
    Vulkan,          ///< Vulkan device
    Metal            ///< Metal device (not yet implemented)
};

/// Texture sampler capabilities
struct SamplerCaps
{
    /// Indicates if device supports border texture addressing mode
    bool is_border_sampling_mode_supported = true;
    /// Indicates if device supports anisotrpoic filtering
    bool is_anisotropic_filtering_supported = true;
    /// Indicates if device supports MIP load bias
    bool is_LOD_bias_supported = true;
};

/// Texture capabilities
struct TextureCaps
{
    /// Indicates if device supports 1D textures
    bool is_texture1D_supported = true;
    /// Indicates if device supports 1D texture arrays
    bool is_texture1D_array_supported = true;
    /// Indicates if device supports 2D multisampled textures
    bool is_Texture2DMS_supported = true;
    /// Indicates if device supports 2D multisampled texture arrays
    bool is_texture2DMS_array_supported = true;
    /// Indicates if device supports texture views
    bool is_texture_view_supported = true;
    /// Indicates if device supports cubemap arrays
    bool is_cubemap_arrays_supported = true;
};

/// Device capabilities
struct DeviceCaps
{
    /// Device type. See evnt::DeviceType.
    DeviceType dev_type = DeviceType::Undefined;
    /// Major API revision. For instance, for D3D11.2 this value would be 11,
    /// and for OpenGL4.3 this value would be 4.
    int32_t major_version = 0;
    /// Major API revision. For instance, for D3D11.2 this value would be 2,
    /// and for OpenGL4.3 this value would be 3.
    int32_t minor_version = 0;
    /// Indicates if device supports separable programs
    bool is_separable_program_supported = true;
    /// Indicates if device supports indirect draw commands
    bool is_indirect_rendering_supported = true;
    /// Indicates if device supports wireframe fill mode
    bool is_wireframe_fill_supported = true;
    /// Indicates if device supports multithreaded resource creation
    bool is_multithreaded_resource_creation_supported = false;
    /// Indicates if device supports compute shaders
    bool is_compute_shaders_supported = true;
    /// Indicates if device supports geometry shaders
    bool is_geometry_shaders_supported = true;
    /// Indicates if device supports tessellation
    bool is_tessellation_supported = true;
    /// Texture sampling capabilities. See evnt::SamplerCaps.
    SamplerCaps sam_caps;
    /// Texture capabilities. See evnt::TextureCaps.
    TextureCaps tex_caps;

    bool isGLDevice() const { return dev_type == DeviceType::OpenGL || dev_type == DeviceType::OpenGLES; }
    bool isD3DDevice() const { return dev_type == DeviceType::D3D11 || dev_type == DeviceType::D3D12; }
    bool isVulkanDevice() const { return dev_type == DeviceType::Vulkan; }

    struct NDCAttribs
    {
        const float min_z;             // Minimum z value of normalized device coordinate space
        const float zto_depth_scale;   // NDC z to depth scale
        const float yto_v_scale;       // Scale to transform NDC y coordinate to texture V coordinate

        float getZtoDepthBias() const
        {
            // Returns ZtoDepthBias such that given NDC z coordinate, depth value can be
            // computed as follows:
            // d = z * ZtoDepthScale + ZtoDepthBias
            return -min_z * zto_depth_scale;
        }
    };

    const NDCAttribs & getNDCAttribs() const
    {
        if(isVulkanDevice())
        {
            // Note that Vulkan itself does not invert Y coordinate when transforming
            // normalized device Y to window space. However, we use negative viewport
            // height which achieves the same effect as in D3D, thererfore we need to
            // invert y (see comments in DeviceContextVkImpl::CommitViewports() for details)
            static constexpr const NDCAttribs ndc_attribs_vk{0.0f, 1.0f, -0.5f};
            return ndc_attribs_vk;
        }
        else if(isD3DDevice())
        {
            static constexpr const NDCAttribs ndc_attribs_d3d{0.0f, 1.0f, -0.5f};
            return ndc_attribs_d3d;
        }
        else if(isGLDevice())
        {
            static constexpr const NDCAttribs ndc_attribs_gl{-1.0f, 0.5f, 0.5f};
            return ndc_attribs_gl;
        }
        else
        {
            static constexpr const NDCAttribs ndc_attribs_default{0.0f, 1.0f, 0.5f};
            return ndc_attribs_default;
        }
    }
};
}   // namespace evnt
#endif   // DEVICECAPS_H
