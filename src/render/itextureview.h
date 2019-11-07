#ifndef ITEXTUREVIEW_H
#define ITEXTUREVIEW_H

#include "ideviceobject.h"

namespace evnt
{
class IDeviceContext;

/// Describes allowed unordered access view mode
enum UAV_ACCESS_FLAG : int32_t
{
    /// Access mode is unspecified
    UAV_ACCESS_UNSPECIFIED = 0x00,
    /// Allow read operations on the UAV
    UAV_ACCESS_FLAG_READ = 0x01,
    /// Allow write operations on the UAV
    UAV_ACCESS_FLAG_WRITE = 0x02,
    /// Allow read and write operations on the UAV
    UAV_ACCESS_FLAG_READ_WRITE = UAV_ACCESS_FLAG_READ | UAV_ACCESS_FLAG_WRITE
};

/// Texture view description
struct TextureViewDesc : DeviceObjectAttribs
{
    static constexpr uint32_t RemainingMipLevels   = static_cast<uint32_t>(-1);
    static constexpr uint32_t RemainingArraySlices = static_cast<uint32_t>(-1);

    /// Describes the texture view type, see evnt::TEXTURE_VIEW_TYPE for details.
    TEXTURE_VIEW_TYPE ViewType = TEXTURE_VIEW_UNDEFINED;
    /// View interpretation of the original texture. For instance,
    /// one slice of a 2D texture array can be viewed as a 2D texture.
    /// See evnt::RESOURCE_DIMENSION for a list of texture types.
    /// If default value evnt::RESOURCE_DIM_UNDEFINED is provided,
    /// the view type will match the type of the referenced texture.
    RESOURCE_DIMENSION TextureDim = RESOURCE_DIM_UNDEFINED;
    /// View format. If default value evnt::TEX_FORMAT_UNKNOWN is provided,
    /// the view format will match the referenced texture format.
    TEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
    /// Most detailed mip level to use
    uint32_t MostDetailedMip = 0;
    /// Total number of mip levels for the view of the texture.
    /// Render target and depth stencil views can address only one mip level.
    /// If 0 is provided, then for a shader resource view all mip levels will be
    /// referenced, and for a render target or a depth stencil view, one mip level
    /// will be referenced.
    uint32_t NumMipLevels = 0;
    union
    {
        /// For a texture array, first array slice to address in the view
        uint32_t FirstArraySlice = 0;
        /// For a 3D texture, first depth slice to address the view
        uint32_t FirstDepthSlice;
    };
    union
    {
        /// For a texture array, number of array slices to address in the view.
        /// Set to 0 to address all array slices.
        uint32_t NumArraySlices = 0;
        /// For a 3D texture, number of depth slices to address in the view
        /// Set to 0 to address all depth slices.
        uint32_t NumDepthSlices;
    };
    /// For an unordered access view, allowed access flags. See evnt::UAV_ACCESS_FLAG
    /// for details.
    uint32_t AccessFlags = 0;

    TextureViewDesc() noexcept {}
    TextureViewDesc(TEXTURE_VIEW_TYPE _ViewType, RESOURCE_DIMENSION _TextureDim,
                    TEXTURE_FORMAT _Format                 = TextureViewDesc{}.Format,
                    uint32_t       _MostDetailedMip        = TextureViewDesc{}.MostDetailedMip,
                    uint32_t       _NumMipLevels           = TextureViewDesc{}.NumMipLevels,
                    uint32_t       _FirstArrayOrDepthSlice = TextureViewDesc{}.FirstArraySlice,
                    uint32_t       _NumArrayOrDepthSlices  = TextureViewDesc{}.NumArraySlices,
                    uint32_t       _AccessFlags            = TextureViewDesc{}.AccessFlags) noexcept :
        ViewType(_ViewType),
        TextureDim(_TextureDim),
        Format(_Format),
        MostDetailedMip(_MostDetailedMip),
        NumMipLevels(_NumMipLevels),
        FirstArraySlice(_FirstArrayOrDepthSlice),
        NumArraySlices(_NumArrayOrDepthSlices),
        AccessFlags(_AccessFlags)
    {}

    /// Tests if two structures are equivalent
    /// \param [in] RHS - reference to the structure to perform comparison with
    /// \return
    /// - True if all members of the two structures are equal.
    /// - False otherwise
    bool operator==(const TextureViewDesc & RHS) const
    {
        // Name is primarily used for debug purposes and does not affect the view.
        // It is ignored in comparison operation.
        return   // strcmp(Name, RHS.Name) == 0            &&
            ViewType == RHS.ViewType && TextureDim == RHS.TextureDim && Format == RHS.Format
            && MostDetailedMip == RHS.MostDetailedMip && NumMipLevels == RHS.NumMipLevels
            && FirstArraySlice == RHS.FirstArraySlice && FirstDepthSlice == RHS.FirstDepthSlice
            && NumArraySlices == RHS.NumArraySlices && NumDepthSlices == RHS.NumDepthSlices
            && AccessFlags == RHS.AccessFlags;
    }
};

/// Texture view interface
/// \remarks
/// To create a texture view, call ITexture::CreateView().
/// Texture view holds strong references to the texture. The texture
/// will not be destroyed until all views are released.
/// The texture view will also keep a strong reference to the texture sampler,
/// if any is set.
class ITextureView : public IDeviceObject
{
public:
    /// Returns the texture view description used to create the object
    const TextureViewDesc & getDesc() const override = 0;

    /// Sets the texture sampler to use for filtering operations
    /// when accessing a texture from shaders. Only
    /// shader resource views can be assigned a sampler.
    /// The view will keep strong reference to the sampler.
    virtual void setSampler(class ISampler * pSampler) = 0;

    /// Returns the pointer to the sampler object set by the ITextureView::SetSampler().
    /// The method does *NOT* call AddRef() on the returned interface,
    /// so Release() must not be called.
    virtual ISampler * getSampler() = 0;

    /// Returns the pointer to the referenced texture object.
    /// The method does *NOT* call AddRef() on the returned interface,
    /// so Release() must not be called.
    virtual class ITexture * getTexture() = 0;
};
}   // namespace evnt

#endif   // ITEXTUREVIEW_H
