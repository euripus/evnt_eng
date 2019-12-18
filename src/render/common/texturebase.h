#ifndef TEXTUREBASE_H
#define TEXTUREBASE_H

#include "../../core/exception.h"
#include "../../log/debug_messages.h"
#include "../utils/graphicsaccessories.h"
#include "./interface/itexture.h"
#include "./interface/itextureview.h"
#include "deviceobjectbase.h"
#include <cassert>

namespace evnt
{
void ValidateTextureDesc(const TextureDesc & TexDesc);
/// Base implementation of the ITexture interface
/// \tparam BaseInterface - base interface that this class will inheret
///                         (evnt::ITextureD3D11, evnt::ITextureD3D12,
///                          evnt::ITextureGL or evnt::ITextureVk).
/// \tparam TRenderDeviceImpl - type of the render device implementation
///                             (evnt::RenderDeviceD3D11Impl, evnt::RenderDeviceD3D12Impl,
///                              evnt::RenderDeviceGLImpl, or evnt::RenderDeviceVkImpl)
/// \tparam TTextureViewImpl - type of the texture view implementation
///                            (evnt::TextureViewD3D11Impl, evnt::TextureViewD3D12Impl,
///                             evnt::TextureViewGLImpl or evnt::TextureViewVkImpl).
template<class BaseInterface, class TRenderDeviceImpl, class TTextureViewImpl>
class TextureBase : public DeviceObjectBase<BaseInterface, TRenderDeviceImpl, TextureDesc>
{
public:
    using TDeviceObjectBase = DeviceObjectBase<BaseInterface, TRenderDeviceImpl, TextureDesc>;

    /// \param pDevice - pointer to the device
    /// \param Desc - texture description
    /// \param bIsDeviceInternal - flag indicating if the texture is an internal device object and
    ///							   must not keep a strong reference to the device
    TextureBase(TRenderDeviceImpl * pDevice, const TextureDesc & Desc, bool bIsDeviceInternal = false) :
        TDeviceObjectBase(pDevice, Desc, bIsDeviceInternal)
    {
        if(this->m_desc.mip_levels == 0)
        {
            // Compute the number of levels in the full mipmap chain
            if(this->m_desc.type == RESOURCE_DIM_TEX_1D || this->m_desc.type == RESOURCE_DIM_TEX_1D_ARRAY)
            {
                this->m_desc.mip_levels = ComputeMipLevelsCount(this->m_desc.width);
            }
            else if(this->m_desc.type == RESOURCE_DIM_TEX_2D || this->m_desc.type == RESOURCE_DIM_TEX_2D_ARRAY
                    || this->m_desc.type == RESOURCE_DIM_TEX_CUBE
                    || this->m_desc.type == RESOURCE_DIM_TEX_CUBE_ARRAY)
            {
                this->m_desc.mip_levels = ComputeMipLevelsCount(this->m_desc.width, this->m_desc.height);
            }
            else if(this->m_desc.type == RESOURCE_DIM_TEX_3D)
            {
                this->m_desc.mip_levels =
                    ComputeMipLevelsCount(this->m_desc.width, this->m_desc.height, this->m_desc.depth);
            }
            else
            {
                UNEXPECTED("Unknown texture type");
            }
        }

        uint64_t DeviceQueuesMask = pDevice->GetCommandQueueMask();
        DEV_CHECK_ERR((this->m_desc.command_queue_mask & DeviceQueuesMask) != 0,
                      "No bits in the command queue mask (0x", std::hex, this->m_desc.command_queue_mask,
                      ") correspond to one of ", pDevice->GetCommandQueueCount(),
                      " available device command queues");
        this->m_desc.command_queue_mask &= DeviceQueuesMask;

        // Validate correctness of texture description
        ValidateTextureDesc(this->m_desc);
    }

    /// Implementaiton of ITexture::CreateView(); calls CreateViewInternal() virtual function that
    /// creates texture view for the specific engine implementation.
    void createView(const struct TextureViewDesc & ViewDesc, ITextureView ** ppView) override
    {
        assert(ViewDesc.view_type != TEXTURE_VIEW_UNDEFINED);   //"Texture view type is not specified"
        if(ViewDesc.view_type == TEXTURE_VIEW_SHADER_RESOURCE)
            assert(this->m_desc.bind_flags
                   & BIND_SHADER_RESOURCE);   //"Attempting to create SRV for texture that was not created
                                              // with BIND_SHADER_RESOURCE flag"
        else if(ViewDesc.view_type == TEXTURE_VIEW_UNORDERED_ACCESS)
            assert(this->m_desc.bind_flags
                   & BIND_UNORDERED_ACCESS);   //"Attempting to create UAV for texture that was not created
                                               // with BIND_UNORDERED_ACCESS flag"
        else if(ViewDesc.view_type == TEXTURE_VIEW_RENDER_TARGET)
            assert(this->m_desc.bind_flags
                   & BIND_RENDER_TARGET);   //"Attempting to create RTV for texture that was not created with
                                            // BIND_RENDER_TARGET flag"
        else if(ViewDesc.view_type == TEXTURE_VIEW_DEPTH_STENCIL)
            assert(this->m_desc.bind_flags
                   & BIND_DEPTH_STENCIL);   //"Attempting to create DSV for texture that was not created with
                                            // BIND_DEPTH_STENCIL flag"
        else
            UNEXPECTED("Unexpected texture view type");

        createViewInternal(ViewDesc, ppView, false);
    }

    /// Creates default texture views.
    /// - Creates default shader resource view addressing the entire texture if evnt::BIND_SHADER_RESOURCE
    /// flag is set.
    /// - Creates default render target view addressing the most detailed mip level if
    /// evnt::BIND_RENDER_TARGET flag is set.
    /// - Creates default depth-stencil view addressing the most detailed mip level if
    /// evnt::BIND_DEPTH_STENCIL flag is set.
    /// - Creates default unordered access view addressing the entire texture if
    /// evnt::BIND_UNORDERED_ACCESS flag is set.
    /// The function calls CreateViewInternal().
    void createDefaultViews();

    virtual void setState(RESOURCE_STATE state) override final { m_state = state; }

    virtual RESOURCE_STATE getState() const override final { return m_state; }

    bool isInKnownState() const { return m_state != RESOURCE_STATE_UNKNOWN; }

    bool CheckState(RESOURCE_STATE state) const
    {
        assert((state & (state - 1)) == 0);   //"Single state is expected"
        assert(isInKnownState());             //"Texture state is unknown"
        return (m_state & state) == state;
    }

protected:
    /// Pure virtual function that creates texture view for the specific engine implementation.
    virtual void createViewInternal(const struct TextureViewDesc & ViewDesc, ITextureView ** ppView,
                                    bool bIsDefaultView) = 0;

    /// Default SRV addressing the entire texture
    std::unique_ptr<TTextureViewImpl> mup_default_srv;
    /// Default RTV addressing the most detailed mip level
    std::unique_ptr<TTextureViewImpl> mup_default_rtv;
    /// Default DSV addressing the most detailed mip level
    std::unique_ptr<TTextureViewImpl> mup_default_dsv;
    /// Default UAV addressing the entire texture
    std::unique_ptr<TTextureViewImpl> mup_default_uav;

    /// Implementation of ITexture::GetDefaultView().
    ITextureView * GetDefaultView(TEXTURE_VIEW_TYPE view_type) override
    {
        switch(view_type)
        {
            case TEXTURE_VIEW_SHADER_RESOURCE:
                return mup_default_srv.get();
            case TEXTURE_VIEW_RENDER_TARGET:
                return mup_default_rtv.get();
            case TEXTURE_VIEW_DEPTH_STENCIL:
                return mup_default_dsv.get();
            case TEXTURE_VIEW_UNORDERED_ACCESS:
                return mup_default_uav.get();
            default:
                UNEXPECTED("Unknown view type");
                return nullptr;
        }
    }

    void correctTextureViewDesc(TextureViewDesc & ViewDesc);

    RESOURCE_STATE m_state = RESOURCE_STATE_UNKNOWN;
};

template<class BaseInterface, class TRenderDeviceImpl, class TTextureViewImpl>
void TextureBase<BaseInterface, TRenderDeviceImpl, TTextureViewImpl>::correctTextureViewDesc(
    TextureViewDesc & ViewDesc)
{
#define TEX_VIEW_VALIDATION_ERROR(...)                                                \
    do                                                                                \
    {                                                                                 \
        std::ostringstream ss;                                                        \
        Log::stream_print(ss, ##__VA_ARGS__);                                         \
        std::string msg = ss.str();                                                   \
        ss.str({});                                                                   \
        ss << "Failed to create texture view '" << ViewDesc.name << "' for texture '" \
           << "': " << msg << std::endl;                                              \
        Log::Log(Log::error, ss.str());                                               \
    } while(false)

    if(!(ViewDesc.view_type > TEXTURE_VIEW_UNDEFINED && ViewDesc.view_type < TEXTURE_VIEW_NUM_VIEWS))
        TEX_VIEW_VALIDATION_ERROR("Texture view type is not specified");

    if(ViewDesc.most_detailed_mip >= this->m_desc.mip_levels)
        TEX_VIEW_VALIDATION_ERROR("Most detailed mip (", ViewDesc.most_detailed_mip,
                                  ") is out of range. The texture has only ", this->m_desc.mip_levels,
                                  " mip ", (this->m_desc.mip_levels > 1 ? "levels." : "level."));

    if(ViewDesc.num_mip_levels != TextureViewDesc::s_remaining_mip_levels
       && ViewDesc.most_detailed_mip + ViewDesc.num_mip_levels > this->m_desc.mip_levels)
        TEX_VIEW_VALIDATION_ERROR("Most detailed mip (", ViewDesc.most_detailed_mip,
                                  ") and number of mip levels in the view (", ViewDesc.num_mip_levels,
                                  ") is out of range. The texture has only ", this->m_desc.mip_levels,
                                  " mip ", (this->m_desc.mip_levels > 1 ? "levels." : "level."));

    if(ViewDesc.format == TEX_FORMAT_UNKNOWN)
        ViewDesc.format =
            GetDefaultTextureViewFormat(this->m_desc.format, ViewDesc.view_type, this->m_desc.bind_flags);

    if(ViewDesc.texture_dim == RESOURCE_DIM_UNDEFINED)
    {
        if(this->m_desc.type == RESOURCE_DIM_TEX_CUBE || this->m_desc.type == RESOURCE_DIM_TEX_CUBE_ARRAY)
        {
            switch(ViewDesc.view_type)
            {
                case TEXTURE_VIEW_SHADER_RESOURCE:
                    ViewDesc.texture_dim = this->m_desc.type;
                    break;

                case TEXTURE_VIEW_RENDER_TARGET:
                case TEXTURE_VIEW_DEPTH_STENCIL:
                case TEXTURE_VIEW_UNORDERED_ACCESS:
                    ViewDesc.texture_dim = RESOURCE_DIM_TEX_2D_ARRAY;
                    break;

                default:
                    UNEXPECTED("Unexpected view type");
            }
        }
        else
        {
            ViewDesc.texture_dim = this->m_desc.type;
        }
    }

    switch(this->m_desc.type)
    {
        case RESOURCE_DIM_TEX_1D:
            if(ViewDesc.texture_dim != RESOURCE_DIM_TEX_1D)
            {
                TEX_VIEW_VALIDATION_ERROR(
                    "Incorrect texture type for Texture 1D view: only Texture 1D is allowed");
            }
            break;

        case RESOURCE_DIM_TEX_1D_ARRAY:
            if(ViewDesc.texture_dim != RESOURCE_DIM_TEX_1D_ARRAY
               && ViewDesc.texture_dim != RESOURCE_DIM_TEX_1D)
            {
                TEX_VIEW_VALIDATION_ERROR("Incorrect view type for Texture 1D Array: only Texture 1D or "
                                          "Texture 1D Array are allowed");
            }
            break;

        case RESOURCE_DIM_TEX_2D:
            if(ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY
               && ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D)
            {
                TEX_VIEW_VALIDATION_ERROR("Incorrect texture type for Texture 2D view: only Texture 2D or "
                                          "Texture 2D Array are allowed");
            }
            break;

        case RESOURCE_DIM_TEX_2D_ARRAY:
            if(ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY
               && ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D)
            {
                TEX_VIEW_VALIDATION_ERROR("Incorrect texture type for Texture 2D Array view: only Texture 2D "
                                          "or Texture 2D Array are allowed");
            }
            break;

        case RESOURCE_DIM_TEX_3D:
            if(ViewDesc.texture_dim != RESOURCE_DIM_TEX_3D)
            {
                TEX_VIEW_VALIDATION_ERROR(
                    "Incorrect texture type for Texture 3D view: only Texture 3D is allowed");
            }
            break;

        case RESOURCE_DIM_TEX_CUBE:
            if(ViewDesc.view_type == TEXTURE_VIEW_SHADER_RESOURCE)
            {
                if(ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D
                   && ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY
                   && ViewDesc.texture_dim != RESOURCE_DIM_TEX_CUBE)
                {
                    TEX_VIEW_VALIDATION_ERROR("Incorrect texture type for Texture cube SRV: Texture 2D, "
                                              "Texture 2D array or Texture Cube is allowed");
                }
            }
            else
            {
                if(ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D
                   && ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY)
                {
                    TEX_VIEW_VALIDATION_ERROR("Incorrect texture type for Texture cube non-shader resource "
                                              "view: Texture 2D or Texture 2D array is allowed");
                }
            }
            break;

        case RESOURCE_DIM_TEX_CUBE_ARRAY:
            if(ViewDesc.view_type == TEXTURE_VIEW_SHADER_RESOURCE)
            {
                if(ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D
                   && ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY
                   && ViewDesc.texture_dim != RESOURCE_DIM_TEX_CUBE
                   && ViewDesc.texture_dim != RESOURCE_DIM_TEX_CUBE_ARRAY)
                {
                    TEX_VIEW_VALIDATION_ERROR(
                        "Incorrect texture type for Texture cube array SRV: Texture 2D, Texture 2D array, "
                        "Texture Cube or Texture Cube Array is allowed");
                }
            }
            else
            {
                if(ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D
                   && ViewDesc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY)
                {
                    TEX_VIEW_VALIDATION_ERROR("Incorrect texture type for Texture cube array non-shader "
                                              "resource view: Texture 2D or Texture 2D array is allowed");
                }
            }
            break;

        default:
            UNEXPECTED("Unexpected texture type");
            break;
    }

    if(ViewDesc.texture_dim == RESOURCE_DIM_TEX_CUBE)
    {
        if(ViewDesc.view_type != TEXTURE_VIEW_SHADER_RESOURCE)
            TEX_VIEW_VALIDATION_ERROR("Unexpected view type: SRV is expected");
        if(ViewDesc.num_array_slices != 6 && ViewDesc.num_array_slices != 0
           && ViewDesc.num_array_slices != TextureViewDesc::s_remaining_array_slices)
            TEX_VIEW_VALIDATION_ERROR("Texture cube SRV is expected to have 6 array slices, while ",
                                      ViewDesc.num_array_slices, " is provided");
        if(ViewDesc.first_array_slice != 0)
            TEX_VIEW_VALIDATION_ERROR("First slice (", ViewDesc.first_array_slice,
                                      ") must be 0 for non-array texture cube SRV");
    }
    if(ViewDesc.texture_dim == RESOURCE_DIM_TEX_CUBE_ARRAY)
    {
        if(ViewDesc.view_type != TEXTURE_VIEW_SHADER_RESOURCE)
            TEX_VIEW_VALIDATION_ERROR("Unexpected view type: SRV is expected");
        if(ViewDesc.num_array_slices != TextureViewDesc::s_remaining_array_slices
           && (ViewDesc.num_array_slices % 6) != 0)
            TEX_VIEW_VALIDATION_ERROR(
                "Number of slices in texture cube array SRV is expected to be multiple of 6. ",
                ViewDesc.num_array_slices, " slices is provided.");
    }

    if(ViewDesc.texture_dim == RESOURCE_DIM_TEX_1D || ViewDesc.texture_dim == RESOURCE_DIM_TEX_2D)
    {
        if(ViewDesc.first_array_slice != 0)
            TEX_VIEW_VALIDATION_ERROR("First slice (", ViewDesc.first_array_slice,
                                      ") must be 0 for non-array texture 1D/2D views");

        if(ViewDesc.num_array_slices != TextureViewDesc::s_remaining_array_slices
           && ViewDesc.num_array_slices > 1)
            TEX_VIEW_VALIDATION_ERROR("Number of slices in the view (", ViewDesc.num_array_slices,
                                      ") must be 1 (or 0) for non-array texture 1D/2D views");
    }
    else if(ViewDesc.texture_dim == RESOURCE_DIM_TEX_1D_ARRAY
            || ViewDesc.texture_dim == RESOURCE_DIM_TEX_2D_ARRAY
            || ViewDesc.texture_dim == RESOURCE_DIM_TEX_CUBE
            || ViewDesc.texture_dim == RESOURCE_DIM_TEX_CUBE_ARRAY)
    {
        if(ViewDesc.first_array_slice >= this->m_desc.array_size)
            TEX_VIEW_VALIDATION_ERROR("First array slice (", ViewDesc.first_array_slice,
                                      ") exceeds the number of slices in the texture array (",
                                      this->m_desc.array_size, ")");

        if(ViewDesc.num_array_slices != TextureViewDesc::s_remaining_array_slices
           && ViewDesc.first_array_slice + ViewDesc.num_array_slices > this->m_desc.array_size)
            TEX_VIEW_VALIDATION_ERROR("First slice (", ViewDesc.first_array_slice,
                                      ") and number of slices in the view (", ViewDesc.num_array_slices,
                                      ") specify more slices than target texture has (",
                                      this->m_desc.array_size, ")");
    }
    else if(ViewDesc.texture_dim == RESOURCE_DIM_TEX_3D)
    {
        auto MipDepth = this->m_desc.depth >> ViewDesc.most_detailed_mip;
        if(ViewDesc.first_depth_slice + ViewDesc.num_depth_slices > MipDepth)
            TEX_VIEW_VALIDATION_ERROR("First slice (", ViewDesc.first_depth_slice,
                                      ") and number of slices in the view (", ViewDesc.num_depth_slices,
                                      ") specify more slices than target 3D texture mip level has (",
                                      MipDepth, ")");
    }
    else
    {
        UNEXPECTED("Unexpected texture dimension");
    }

    if(GetTextureFormatInfo(ViewDesc.format).is_typeless)
    {
        TEX_VIEW_VALIDATION_ERROR("Texture view format (", GetTextureFormatInfo(ViewDesc.format).name,
                                  ") cannot be typeless");
    }

#undef TEX_VIEW_VALIDATION_ERROR

    if(ViewDesc.num_mip_levels == 0 || ViewDesc.num_mip_levels == TextureViewDesc::s_remaining_mip_levels)
    {
        if(ViewDesc.view_type == TEXTURE_VIEW_SHADER_RESOURCE)
            ViewDesc.num_mip_levels = this->m_desc.mip_levels - ViewDesc.most_detailed_mip;
        else
            ViewDesc.num_mip_levels = 1;
    }

    if(ViewDesc.num_array_slices == 0
       || ViewDesc.num_array_slices == TextureViewDesc::s_remaining_array_slices)
    {
        if(ViewDesc.texture_dim == RESOURCE_DIM_TEX_1D_ARRAY
           || ViewDesc.texture_dim == RESOURCE_DIM_TEX_2D_ARRAY
           || ViewDesc.texture_dim == RESOURCE_DIM_TEX_CUBE
           || ViewDesc.texture_dim == RESOURCE_DIM_TEX_CUBE_ARRAY)
            ViewDesc.num_array_slices = this->m_desc.array_size - ViewDesc.first_array_slice;
        else if(ViewDesc.texture_dim == RESOURCE_DIM_TEX_3D)
        {
            auto MipDepth             = this->m_desc.depth >> ViewDesc.most_detailed_mip;
            ViewDesc.num_depth_slices = MipDepth - ViewDesc.first_depth_slice;
        }
        else
            ViewDesc.num_array_slices = 1;
    }

    if((ViewDesc.view_type == TEXTURE_VIEW_RENDER_TARGET)
       && (ViewDesc.format == TEX_FORMAT_R8_SNORM || ViewDesc.format == TEX_FORMAT_RG8_SNORM
           || ViewDesc.format == TEX_FORMAT_RGBA8_SNORM || ViewDesc.format == TEX_FORMAT_R16_SNORM
           || ViewDesc.format == TEX_FORMAT_RG16_SNORM || ViewDesc.format == TEX_FORMAT_RGBA16_SNORM))
    {
        const auto         FmtName = GetTextureFormatInfo(ViewDesc.format).name;
        std::ostringstream ss;
        ss << FmtName
           << " render target view is created.\n There might be an issue in OpenGL driver on NVidia "
              "hardware: when rendering to SNORM textures, all negative values are clamped to zero. Use "
              "UNORM format instead."
           << std::endl;
        Log::Log(Log::warning, ss.str());
    }
}

template<class BaseInterface, class TRenderDeviceImpl, class TTextureViewImpl>
void TextureBase<BaseInterface, TRenderDeviceImpl, TTextureViewImpl>::createDefaultViews()
{
    const auto & TexFmtAttribs = GetTextureFormatInfo(this->m_Desc.Format);
    if(TexFmtAttribs.ComponentType == COMPONENT_TYPE_UNDEFINED)
    {
        // Cannot create default view for TYPELESS formats
        return;
    }

    if(this->m_desc.bind_flags & BIND_SHADER_RESOURCE)
    {
        TextureViewDesc ViewDesc;
        ViewDesc.view_type  = TEXTURE_VIEW_SHADER_RESOURCE;
        ITextureView * pSRV = nullptr;
        createViewInternal(ViewDesc, &pSRV, true);
        mup_default_srv.reset(static_cast<TTextureViewImpl *>(pSRV));
        assert(mup_default_srv->getDesc().view_type
               == TEXTURE_VIEW_SHADER_RESOURCE);   // "Unexpected view type"
    }

    if(this->m_desc.bind_flags & BIND_RENDER_TARGET)
    {
        TextureViewDesc ViewDesc;
        ViewDesc.view_type  = TEXTURE_VIEW_RENDER_TARGET;
        ITextureView * pRTV = nullptr;
        createViewInternal(ViewDesc, &pRTV, true);
        mup_default_rtv.reset(static_cast<TTextureViewImpl *>(pRTV));
        assert(mup_default_rtv->getDesc().view_type == TEXTURE_VIEW_RENDER_TARGET);   //"Unexpected view type"
    }

    if(this->m_desc.bind_flags & BIND_DEPTH_STENCIL)
    {
        TextureViewDesc ViewDesc;
        ViewDesc.view_type  = TEXTURE_VIEW_DEPTH_STENCIL;
        ITextureView * pDSV = nullptr;
        createViewInternal(ViewDesc, &pDSV, true);
        mup_default_dsv.reset(static_cast<TTextureViewImpl *>(pDSV));
        assert(mup_default_dsv->getDesc().view_type == TEXTURE_VIEW_DEPTH_STENCIL);   //"Unexpected view type"
    }

    if(this->m_desc.bind_flags & BIND_UNORDERED_ACCESS)
    {
        TextureViewDesc ViewDesc;
        ViewDesc.view_type    = TEXTURE_VIEW_UNORDERED_ACCESS;
        ViewDesc.access_flags = UAV_ACCESS_FLAG_READ_WRITE;
        ITextureView * pUAV   = nullptr;
        createViewInternal(ViewDesc, &pUAV, true);
        mup_default_uav.reset(static_cast<TTextureViewImpl *>(pUAV));
        assert(mup_default_uav->getDesc().view_type
               == TEXTURE_VIEW_UNORDERED_ACCESS);   //"Unexpected view type"
    }
}
}   // namespace evnt
#endif   // TEXTUREBASE_H
