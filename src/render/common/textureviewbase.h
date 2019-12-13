#ifndef TEXTUREVIEWBASE_H
#define TEXTUREVIEWBASE_H

#include "./interface/itextureview.h"
#include "deviceobjectbase.h"

namespace evnt
{
template<typename BaseInterface, typename RenderDeviceImplType>
class TextureViewBase : public DeviceObjectBase<BaseInterface, RenderDeviceImplType, TextureViewDesc>
{
public:
    using TDeviceObjectBase = DeviceObjectBase<BaseInterface, RenderDeviceImplType, TextureViewDesc>;

    /// \param pDevice - pointer to the render device.
    /// \param ViewDesc - texture view description.
    /// \param pTexture - pointer to the texture that the view is to be created for.
    /// \param bIsDefaultView - flag indicating if the view is default view, and is thus
    ///                         part of the texture object. In this case the view will
    ///                         attach to the texture's reference counters.
    TextureViewBase(RenderDeviceImplType * pDevice, const TextureViewDesc & ViewDesc,
                    class ITexture * pTexture, bool bIsDefaultView) :
        // Default views are created as part of the texture, so we cannot not keep strong
        // reference to the texture to avoid cyclic links. Instead, we will attach to the
        // reference counters of the texture.
        TDeviceObjectBase(pDevice, ViewDesc),
        mp_texture(pTexture)   //,
    // For non-default view, we will keep strong reference to texture
    // m_spTexture(bIsDefaultView ? nullptr : pTexture)
    {}

    // Implementation of ITextureView::SetSampler()
    virtual void setSampler(std::unique_ptr<ISampler> sampler) override final
    {
        mup_sampler = std::move(sampler);
    }

    /// Implementation of ITextureView::GetSampler()
    ISampler * getSampler() override final { return mup_sampler.get(); }
    const ISampler *   getSampler() const { return mup_sampler.get(); }

    /// Implementation of ITextureView::GetTexture()
    ITexture * getTexture() override final { return mp_texture; }
    const ITexture *   getTexture() const { return mp_texture; }

    template<typename TextureType>
    TextureType * getTexture()
    {
        return static_cast<TextureType>(mp_texture);
    }

    template<typename TextureType>
    TextureType * getTexture() const
    {
        return static_cast<TextureType>(mp_texture);
    }

protected:
    /// Strong reference to the sampler
    std::unique_ptr<ISampler> mup_sampler;
    /// Raw pointer to the texture
    ITexture * const mp_texture;
    /// Strong reference to the texture. Used for non-default views
    /// to keep the texture alive
    // Diligent::RefCntAutoPtr<ITexture> m_spTexture;
};
}   // namespace evnt

#endif   // TEXTUREVIEWBASE_H
