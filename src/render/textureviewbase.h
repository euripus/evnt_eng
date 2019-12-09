#ifndef TEXTUREVIEWBASE_H
#define TEXTUREVIEWBASE_H

#include "./interface/itextureview.h"
#include <memory>

namespace evnt
{
class TextureViewBase : public ITextureView
{
public:
    // Implementation of ITextureView::SetSampler()
    virtual void setSampler(class ISampler * sampler) override final{}// { mup_sampler = sampler; }

    /// Implementation of ITextureView::GetSampler()
    virtual ISampler * getSampler() override final { return mup_sampler.get(); }
    const ISampler *   getSampler() const { return mup_sampler.get(); }

    /// Implementation of ITextureView::GetTexture()
    virtual ITexture * getTexture() override final { return mp_texture; }
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
