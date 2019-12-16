#ifndef RENDERDEVICEBASE_H
#define RENDERDEVICEBASE_H

#include "../utils/graphicsaccessories.h"
#include "./interface/idevicecontext.h"
#include "./interface/isampler.h"
#include "devicecaps.h"
#include "objectbase.h"
#include <memory>
#include <vector>

namespace evnt
{
/// Base implementation of a render device
/// \tparam BaseInterface - base interface that this class will inheret.
/// \warning
/// Render device must *NOT* hold strong references to any
/// object it creates to avoid circular dependencies.
/// Device context, swap chain and all object the device creates
/// keep strong reference to the device.
/// Device only holds weak reference to the immediate context.
template<typename BaseInterface>
class RenderDeviceBase : public ObjectBase<BaseInterface>
{
protected:
    DeviceCaps                     m_device_caps;
    std::vector<TextureFormatInfo> m_texture_formats_info;
    std::vector<bool>              m_texture_formats_info_init_flags;
    /// Weak reference to the immediate context. Immediate context holds strong reference
    /// to the device, so we must use weak reference to avoid circular dependencies.
    std::weak_ptr<IDeviceContext> mwp_immediate_context;
    /// Weak references to deferred contexts.
    std::vector<std::weak_ptr<IDeviceContext>> m_deferred_contexts;

    virtual void testTextureFormat(TextureFormatInfo & tex_format) = 0;

public:
    using TObjectBase = ObjectBase<BaseInterface>;

    RenderDeviceBase(uint32_t num_deferred_contexts) : TObjectBase()
    {
        m_deferred_contexts.resize(num_deferred_contexts);

        // Initialize texture format info
        for(uint32_t fmt = TEX_FORMAT_UNKNOWN; fmt < TEX_FORMAT_NUM_FORMATS; ++fmt)
            m_texture_formats_info[fmt] = GetTextureFormatInfo(static_cast<TEXTURE_FORMAT>(fmt));

        // https://msdn.microsoft.com/en-us/library/windows/desktop/ff471325(v=vs.85).aspx
        TEXTURE_FORMAT FilterableFormats[] = {
            TEX_FORMAT_RGBA32_FLOAT,   // OpenGL ES3.1 does not require this format to be filterable
            TEX_FORMAT_RGBA16_FLOAT, TEX_FORMAT_RGBA16_UNORM, TEX_FORMAT_RGBA16_SNORM,
            TEX_FORMAT_RG32_FLOAT,   // OpenGL ES3.1 does not require this format to be filterable
            TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS,
            // TEX_FORMAT_R10G10B10A2_UNORM,
            TEX_FORMAT_R11G11B10_FLOAT, TEX_FORMAT_RGBA8_UNORM, TEX_FORMAT_RGBA8_UNORM_SRGB,
            TEX_FORMAT_RGBA8_SNORM, TEX_FORMAT_RG16_FLOAT, TEX_FORMAT_RG16_UNORM, TEX_FORMAT_RG16_SNORM,
            TEX_FORMAT_R32_FLOAT,   // OpenGL ES3.1 does not require this format to be filterable
            TEX_FORMAT_R24_UNORM_X8_TYPELESS, TEX_FORMAT_RG8_UNORM, TEX_FORMAT_RG8_SNORM,
            TEX_FORMAT_R16_FLOAT, TEX_FORMAT_R16_UNORM, TEX_FORMAT_R16_SNORM, TEX_FORMAT_R8_UNORM,
            TEX_FORMAT_R8_SNORM, TEX_FORMAT_A8_UNORM, TEX_FORMAT_RGB9E5_SHAREDEXP, TEX_FORMAT_RG8_B8G8_UNORM,
            TEX_FORMAT_G8R8_G8B8_UNORM, TEX_FORMAT_BC1_UNORM, TEX_FORMAT_BC1_UNORM_SRGB, TEX_FORMAT_BC2_UNORM,
            TEX_FORMAT_BC2_UNORM_SRGB, TEX_FORMAT_BC3_UNORM, TEX_FORMAT_BC3_UNORM_SRGB, TEX_FORMAT_BC4_UNORM,
            TEX_FORMAT_BC4_SNORM, TEX_FORMAT_BC5_UNORM, TEX_FORMAT_BC5_SNORM, TEX_FORMAT_B5G6R5_UNORM};
        for(uint32_t fmt = 0; fmt < std::size(FilterableFormats); ++fmt)
            m_texture_formats_info[FilterableFormats[fmt]].filterable = true;
    }

    /// Implementation of IRenderDevice::CreateResourceMapping().
    virtual void createResourceMapping(const ResourceMappingDesc & MappingDesc,
                                       IResourceMapping **         ppMapping) override final;

    /// Implementation of IRenderDevice::GetDeviceCaps().
    virtual const DeviceCaps & getDeviceCaps() const override final { return m_device_caps; }

    /// Implementation of IRenderDevice::GetTextureFormatInfo().
    virtual const TextureFormatInfo & getTextureFormatInfo(TEXTURE_FORMAT tex_format) override final
    {
        assert(tex_format >= TEX_FORMAT_UNKNOWN
               && tex_format < TEX_FORMAT_NUM_FORMATS);   // "Texture format out of range
        const auto & tex_fmt_info = m_texture_formats_info[tex_format];
        assert(tex_fmt_info.format == tex_format);   //"Sanity check failed"
        if(!m_texture_formats_info_init_flags[tex_format])
        {
            if(tex_fmt_info.supported)
                testTextureFormat(tex_format);
            m_texture_formats_info_init_flags[tex_format] = true;
        }
        return tex_fmt_info;
    }

    /// Set weak reference to the immediate context
    void setImmediateContext(std::shared_ptr<IDeviceContext> sp_immediate_context)
    {
        assert(!mwp_immediate_context.lock());   // "Immediate context has already been set"
        mwp_immediate_context = sp_immediate_context;
    }

    /// Set weak reference to the deferred context
    void setDeferredContext(size_t ctx, std::shared_ptr<IDeviceContext> sp_deferred_ctx)
    {
        assert(!m_deferred_contexts[ctx].lock());   // "Deferred context has already been set"
        m_deferred_contexts[ctx] = sp_deferred_ctx;
    }

    /// Returns number of deferred contexts
    size_t getNumDeferredContexts() const { return m_deferred_contexts.size(); }

    std::shared_ptr<IDeviceContext> getImmediateContext() { return mwp_immediate_context.lock(); }
    std::shared_ptr<IDeviceContext> getDeferredContext(size_t ctx) { return m_deferred_contexts[ctx].lock(); }
};
}   // namespace evnt

#endif   // RENDERDEVICEBASE_H
