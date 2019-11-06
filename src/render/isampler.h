#ifndef ISAMPLER_H
#define ISAMPLER_H

#include "ideviceobject.h"

namespace evnt
{
/// Sampler description
/// This structure describes the sampler state which is used in a call to
/// IRenderDevice::CreateSampler() to create a sampler object.
///
/// To create an anisotropic filter, all three filters must either be evnt::FILTER_TYPE_ANISOTROPIC
/// or evnt::FILTER_TYPE_COMPARISON_ANISOTROPIC.
///
/// MipFilter cannot be comparison filter except for evnt::FILTER_TYPE_ANISOTROPIC if all
/// three filters have that value.
///
/// Both MinFilter and MagFilter must either be regular filters or comparison filters.
/// Mixing comparison and regular filters is an error.
struct SamplerDesc : public DeviceObjectAttribs
{
    /// Texture minification filter, see evnt::FILTER_TYPE for details.
    /// Default value: evnt::FILTER_TYPE_LINEAR.
    FILTER_TYPE min_filter = FILTER_TYPE_LINEAR;
    /// Texture magnification filter, see evnt::FILTER_TYPE for details.
    /// Default value: evnt::FILTER_TYPE_LINEAR.
    FILTER_TYPE mag_filter = FILTER_TYPE_LINEAR;
    /// Mip filter, see evnt::FILTER_TYPE for details.
    /// Only FILTER_TYPE_POINT, FILTER_TYPE_LINEAR, FILTER_TYPE_ANISOTROPIC, and
    /// FILTER_TYPE_COMPARISON_ANISOTROPIC are allowed.
    /// Default value: evnt::FILTER_TYPE_LINEAR.
    FILTER_TYPE mip_filter = FILTER_TYPE_LINEAR;
    /// Texture address mode for U coordinate, see evnt::TEXTURE_ADDRESS_MODE for details
    /// Default value: evnt::TEXTURE_ADDRESS_CLAMP.
    TEXTURE_ADDRESS_MODE address_u = TEXTURE_ADDRESS_CLAMP;
    /// Texture address mode for V coordinate, see evnt::TEXTURE_ADDRESS_MODE for details
    /// Default value: evnt::TEXTURE_ADDRESS_CLAMP.
    TEXTURE_ADDRESS_MODE address_v = TEXTURE_ADDRESS_CLAMP;
    /// Texture address mode for W coordinate, see evnt::TEXTURE_ADDRESS_MODE for details
    /// Default value: evnt::TEXTURE_ADDRESS_CLAMP.
    TEXTURE_ADDRESS_MODE address_w = TEXTURE_ADDRESS_CLAMP;
    /// Offset from the calculated mipmap level. For example, if a sampler calculates that a texture
    /// should be sampled at mipmap level 1.2 and MipLODBias is 2.3, then the texture will be sampled at
    /// mipmap level 3.5. Default value: 0.
    float mip_lod_bias = 0;
    /// Maximum anisotropy level for the anisotropic filter. Default value: 0.
    uint32_t max_anisotropy = 0;
    /// A function that compares sampled data against existing sampled data when comparsion
    /// filter is used. Default value: evnt::COMPARISON_FUNC_NEVER.
    COMPARISON_FUNCTION comparison_func = COMPARISON_FUNC_NEVER;
    /// Border color to use if TEXTURE_ADDRESS_BORDER is specified for AddressU, AddressV, or AddressW.
    /// Default value: {0,0,0,0}
    float border_color[4] = {0, 0, 0, 0};
    /// Specifies the minimum value that LOD is clamped to before accessing the texture MIP levels.
    /// Must be less than or equal to MaxLOD.
    /// Default value: 0.
    float min_lod = 0;
    /// Specifies the maximum value that LOD is clamped to before accessing the texture MIP levels.
    /// Must be greater than or equal to MinLOD.
    /// Default value: +FLT_MAX.
    float max_lod = +3.402823466e+38F;

    SamplerDesc() noexcept {}
    SamplerDesc(FILTER_TYPE _MinFilter, FILTER_TYPE _MagFilter, FILTER_TYPE _MipFilter,
                TEXTURE_ADDRESS_MODE _AddressU       = SamplerDesc{}.address_u,
                TEXTURE_ADDRESS_MODE _AddressV       = SamplerDesc{}.address_v,
                TEXTURE_ADDRESS_MODE _AddressW       = SamplerDesc{}.address_w,
                float                _MipLODBias     = SamplerDesc{}.mip_lod_bias,
                uint32_t             _MaxAnisotropy  = SamplerDesc{}.max_anisotropy,
                COMPARISON_FUNCTION  _ComparisonFunc = SamplerDesc{}.comparison_func,
                float _MinLOD = SamplerDesc{}.min_lod, float _MaxLOD = SamplerDesc{}.max_lod) :
        min_filter(_MinFilter),
        mag_filter(_MagFilter),
        mip_filter(_MipFilter),
        address_u(_AddressU),
        address_v(_AddressV),
        address_w(_AddressW),
        mip_lod_bias(_MipLODBias),
        max_anisotropy(_MaxAnisotropy),
        comparison_func(_ComparisonFunc),
        min_lod(_MinLOD),
        max_lod(_MaxLOD)
    {
        border_color[0] = border_color[1] = border_color[2] = border_color[3] = 0;
    }

    /// Tests if two structures are equivalent
    /// \param [in] RHS - reference to the structure to perform comparison with
    /// \return
    /// - True if all members of the two structures are equal.
    /// - False otherwise.
    /// The operator ignores DeviceObjectAttribs::Name field as it does not affect
    /// the sampler state.
    bool operator==(const SamplerDesc & RHS) const
    {
        // Name is primarily used for debug purposes and does not affect the state.
        // It is ignored in comparison operation.
        return min_filter == RHS.min_filter && mag_filter == RHS.mag_filter && mip_filter == RHS.mip_filter
               && address_u == RHS.address_u && address_v == RHS.address_v && address_w == RHS.address_w
               && mip_lod_bias == RHS.mip_lod_bias && max_anisotropy == RHS.max_anisotropy
               && comparison_func == RHS.comparison_func && border_color[0] == RHS.border_color[0]
               && border_color[1] == RHS.border_color[1] && border_color[2] == RHS.border_color[2]
               && border_color[3] == RHS.border_color[3] && min_lod == RHS.min_lod && max_lod == RHS.max_lod;
    }
};

/// Texture sampler interface.
/// The interface holds the sampler state that can be used to perform texture filtering.
/// To create a sampler, call IRenderDevice::CreateSampler(). To use a sampler,
/// call ITextureView::SetSampler().
class ISampler : public IDeviceObject
{
public:
    /// Returns the sampler description used to create the object
    const SamplerDesc & getDesc() const override = 0;
};
}   // namespace evnt

#endif   // ISAMPLER_H
