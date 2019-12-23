#ifndef TEXTUREBASE_H
#define TEXTUREBASE_H

#include "../../log/debug_messages.h"
#include "../utils/graphicsaccessories.h"
#include "./interface/itexture.h"
#include "./interface/itextureview.h"
#include "deviceobjectbase.h"
#include <cassert>

namespace evnt
{
void ValidateTextureDesc(const TextureDesc & tex_desc);
void ValidateUpdateTextureParams(const TextureDesc & tex_desc, uint32_t mip_level, uint32_t slice,
                                 const class Box & dst_box, const TextureSubResData & subres_data);
void ValidateCopyTextureParams(const class CopyTextureAttribs & copy_attribs);
void ValidateMapTextureParams(const TextureDesc & tex_desc, uint32_t mip_level, uint32_t array_slice,
                              MAP_TYPE MapType, uint32_t MapFlags, const Box * p_map_region);

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
template<typename BaseInterface, typename TRenderDeviceImpl, typename TTextureViewImpl>
class TextureBase : public DeviceObjectBase<BaseInterface, TRenderDeviceImpl, TextureDesc>
{
public:
    using TDeviceObjectBase = DeviceObjectBase<BaseInterface, TRenderDeviceImpl, TextureDesc>;

    /// \param p_device - pointer to the device
    /// \param desc - texture description
    TextureBase(TRenderDeviceImpl * p_device, const TextureDesc & desc) : TDeviceObjectBase(p_device, desc)
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

        uint64_t device_queues_mask = p_device->getCommandQueueMask();
        assert((this->m_desc.command_queue_mask & device_queues_mask)
               != 0);   // "No bits in the command queue mask correspond to one of available device command
                        // queues"
        this->m_desc.command_queue_mask &= device_queues_mask;

        // Validate correctness of texture description
        ValidateTextureDesc(this->m_desc);
    }

    /// Implementaiton of ITexture::CreateView(); calls CreateViewInternal() virtual function that
    /// creates texture view for the specific engine implementation.
    void createView(const struct TextureViewDesc & view_desc, ITextureView ** pp_view) override
    {
        assert(view_desc.view_type != TEXTURE_VIEW_UNDEFINED);   //"Texture view type is not specified"
        if(view_desc.view_type == TEXTURE_VIEW_SHADER_RESOURCE)
            assert(this->m_desc.bind_flags
                   & BIND_SHADER_RESOURCE);   //"Attempting to create SRV for texture that was not created
                                              // with BIND_SHADER_RESOURCE flag"
        else if(view_desc.view_type == TEXTURE_VIEW_UNORDERED_ACCESS)
            assert(this->m_desc.bind_flags
                   & BIND_UNORDERED_ACCESS);   //"Attempting to create UAV for texture that was not created
                                               // with BIND_UNORDERED_ACCESS flag"
        else if(view_desc.view_type == TEXTURE_VIEW_RENDER_TARGET)
            assert(this->m_desc.bind_flags
                   & BIND_RENDER_TARGET);   //"Attempting to create RTV for texture that was not created with
                                            // BIND_RENDER_TARGET flag"
        else if(view_desc.view_type == TEXTURE_VIEW_DEPTH_STENCIL)
            assert(this->m_desc.bind_flags
                   & BIND_DEPTH_STENCIL);   //"Attempting to create DSV for texture that was not created with
                                            // BIND_DEPTH_STENCIL flag"
        else
            UNEXPECTED("Unexpected texture view type");

        createViewInternal(view_desc, pp_view, false);
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

    void setState(RESOURCE_STATE state) override final { m_state = state; }

    RESOURCE_STATE getState() const override final { return m_state; }

    bool isInKnownState() const { return m_state != RESOURCE_STATE_UNKNOWN; }

    bool checkState(RESOURCE_STATE state) const
    {
        assert((state & (state - 1)) == 0);   //"Single state is expected"
        assert(isInKnownState());             //"Texture state is unknown"
        return (m_state & state) == state;
    }

protected:
    /// Pure virtual function that creates texture view for the specific engine implementation.
    virtual void createViewInternal(const struct TextureViewDesc & view_desc, ITextureView ** pp_view,
                                    bool is_default_view) = 0;

    /// Default SRV addressing the entire texture
    std::unique_ptr<TTextureViewImpl> mup_default_srv;
    /// Default RTV addressing the most detailed mip level
    std::unique_ptr<TTextureViewImpl> mup_default_rtv;
    /// Default DSV addressing the most detailed mip level
    std::unique_ptr<TTextureViewImpl> mup_default_dsv;
    /// Default UAV addressing the entire texture
    std::unique_ptr<TTextureViewImpl> mup_default_uav;

    /// Implementation of ITexture::GetDefaultView().
    ITextureView * getDefaultView(TEXTURE_VIEW_TYPE view_type) override
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

    void correctTextureViewDesc(TextureViewDesc & view_desc);

    RESOURCE_STATE m_state = RESOURCE_STATE_UNKNOWN;
};

template<typename BaseInterface, typename TRenderDeviceImpl, typename TTextureViewImpl>
void TextureBase<BaseInterface, TRenderDeviceImpl, TTextureViewImpl>::correctTextureViewDesc(
    TextureViewDesc & view_desc)
{
#define TEX_VIEW_VALIDATION_ERROR(...)                                                 \
    do                                                                                 \
    {                                                                                  \
        std::ostringstream ss;                                                         \
        Log::stream_print(ss, ##__VA_ARGS__);                                          \
        std::string msg = ss.str();                                                    \
        ss.str({});                                                                    \
        ss << "Failed to create texture view '" << view_desc.name << "' for texture '" \
           << "': " << msg << std::endl;                                               \
        Log::Log(Log::error, ss.str());                                                \
    } while(false)

    if(!(view_desc.view_type > TEXTURE_VIEW_UNDEFINED && view_desc.view_type < TEXTURE_VIEW_NUM_VIEWS))
        TEX_VIEW_VALIDATION_ERROR("Texture view type is not specified");

    if(view_desc.most_detailed_mip >= this->m_desc.mip_levels)
        TEX_VIEW_VALIDATION_ERROR("Most detailed mip (", view_desc.most_detailed_mip,
                                  ") is out of range. The texture has only ", this->m_desc.mip_levels,
                                  " mip ", (this->m_desc.mip_levels > 1 ? "levels." : "level."));

    if(view_desc.num_mip_levels != TextureViewDesc::s_remaining_mip_levels
       && view_desc.most_detailed_mip + view_desc.num_mip_levels > this->m_desc.mip_levels)
        TEX_VIEW_VALIDATION_ERROR("Most detailed mip (", view_desc.most_detailed_mip,
                                  ") and number of mip levels in the view (", view_desc.num_mip_levels,
                                  ") is out of range. The texture has only ", this->m_desc.mip_levels,
                                  " mip ", (this->m_desc.mip_levels > 1 ? "levels." : "level."));

    if(view_desc.format == TEX_FORMAT_UNKNOWN)
        view_desc.format =
            GetDefaultTextureViewFormat(this->m_desc.format, view_desc.view_type, this->m_desc.bind_flags);

    if(view_desc.texture_dim == RESOURCE_DIM_UNDEFINED)
    {
        if(this->m_desc.type == RESOURCE_DIM_TEX_CUBE || this->m_desc.type == RESOURCE_DIM_TEX_CUBE_ARRAY)
        {
            switch(view_desc.view_type)
            {
                case TEXTURE_VIEW_SHADER_RESOURCE:
                    view_desc.texture_dim = this->m_desc.type;
                    break;

                case TEXTURE_VIEW_RENDER_TARGET:
                case TEXTURE_VIEW_DEPTH_STENCIL:
                case TEXTURE_VIEW_UNORDERED_ACCESS:
                    view_desc.texture_dim = RESOURCE_DIM_TEX_2D_ARRAY;
                    break;

                default:
                    UNEXPECTED("Unexpected view type");
            }
        }
        else
        {
            view_desc.texture_dim = this->m_desc.type;
        }
    }

    switch(this->m_desc.type)
    {
        case RESOURCE_DIM_TEX_1D:
            if(view_desc.texture_dim != RESOURCE_DIM_TEX_1D)
            {
                TEX_VIEW_VALIDATION_ERROR(
                    "Incorrect texture type for Texture 1D view: only Texture 1D is allowed");
            }
            break;

        case RESOURCE_DIM_TEX_1D_ARRAY:
            if(view_desc.texture_dim != RESOURCE_DIM_TEX_1D_ARRAY
               && view_desc.texture_dim != RESOURCE_DIM_TEX_1D)
            {
                TEX_VIEW_VALIDATION_ERROR("Incorrect view type for Texture 1D Array: only Texture 1D or "
                                          "Texture 1D Array are allowed");
            }
            break;

        case RESOURCE_DIM_TEX_2D:
            if(view_desc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY
               && view_desc.texture_dim != RESOURCE_DIM_TEX_2D)
            {
                TEX_VIEW_VALIDATION_ERROR("Incorrect texture type for Texture 2D view: only Texture 2D or "
                                          "Texture 2D Array are allowed");
            }
            break;

        case RESOURCE_DIM_TEX_2D_ARRAY:
            if(view_desc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY
               && view_desc.texture_dim != RESOURCE_DIM_TEX_2D)
            {
                TEX_VIEW_VALIDATION_ERROR("Incorrect texture type for Texture 2D Array view: only Texture 2D "
                                          "or Texture 2D Array are allowed");
            }
            break;

        case RESOURCE_DIM_TEX_3D:
            if(view_desc.texture_dim != RESOURCE_DIM_TEX_3D)
            {
                TEX_VIEW_VALIDATION_ERROR(
                    "Incorrect texture type for Texture 3D view: only Texture 3D is allowed");
            }
            break;

        case RESOURCE_DIM_TEX_CUBE:
            if(view_desc.view_type == TEXTURE_VIEW_SHADER_RESOURCE)
            {
                if(view_desc.texture_dim != RESOURCE_DIM_TEX_2D
                   && view_desc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY
                   && view_desc.texture_dim != RESOURCE_DIM_TEX_CUBE)
                {
                    TEX_VIEW_VALIDATION_ERROR("Incorrect texture type for Texture cube SRV: Texture 2D, "
                                              "Texture 2D array or Texture Cube is allowed");
                }
            }
            else
            {
                if(view_desc.texture_dim != RESOURCE_DIM_TEX_2D
                   && view_desc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY)
                {
                    TEX_VIEW_VALIDATION_ERROR("Incorrect texture type for Texture cube non-shader resource "
                                              "view: Texture 2D or Texture 2D array is allowed");
                }
            }
            break;

        case RESOURCE_DIM_TEX_CUBE_ARRAY:
            if(view_desc.view_type == TEXTURE_VIEW_SHADER_RESOURCE)
            {
                if(view_desc.texture_dim != RESOURCE_DIM_TEX_2D
                   && view_desc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY
                   && view_desc.texture_dim != RESOURCE_DIM_TEX_CUBE
                   && view_desc.texture_dim != RESOURCE_DIM_TEX_CUBE_ARRAY)
                {
                    TEX_VIEW_VALIDATION_ERROR(
                        "Incorrect texture type for Texture cube array SRV: Texture 2D, Texture 2D array, "
                        "Texture Cube or Texture Cube Array is allowed");
                }
            }
            else
            {
                if(view_desc.texture_dim != RESOURCE_DIM_TEX_2D
                   && view_desc.texture_dim != RESOURCE_DIM_TEX_2D_ARRAY)
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

    if(view_desc.texture_dim == RESOURCE_DIM_TEX_CUBE)
    {
        if(view_desc.view_type != TEXTURE_VIEW_SHADER_RESOURCE)
            TEX_VIEW_VALIDATION_ERROR("Unexpected view type: SRV is expected");
        if(view_desc.num_array_slices != 6 && view_desc.num_array_slices != 0
           && view_desc.num_array_slices != TextureViewDesc::s_remaining_array_slices)
            TEX_VIEW_VALIDATION_ERROR("Texture cube SRV is expected to have 6 array slices, while ",
                                      view_desc.num_array_slices, " is provided");
        if(view_desc.first_array_slice != 0)
            TEX_VIEW_VALIDATION_ERROR("First slice (", view_desc.first_array_slice,
                                      ") must be 0 for non-array texture cube SRV");
    }
    if(view_desc.texture_dim == RESOURCE_DIM_TEX_CUBE_ARRAY)
    {
        if(view_desc.view_type != TEXTURE_VIEW_SHADER_RESOURCE)
            TEX_VIEW_VALIDATION_ERROR("Unexpected view type: SRV is expected");
        if(view_desc.num_array_slices != TextureViewDesc::s_remaining_array_slices
           && (view_desc.num_array_slices % 6) != 0)
            TEX_VIEW_VALIDATION_ERROR(
                "Number of slices in texture cube array SRV is expected to be multiple of 6. ",
                view_desc.num_array_slices, " slices is provided.");
    }

    if(view_desc.texture_dim == RESOURCE_DIM_TEX_1D || view_desc.texture_dim == RESOURCE_DIM_TEX_2D)
    {
        if(view_desc.first_array_slice != 0)
            TEX_VIEW_VALIDATION_ERROR("First slice (", view_desc.first_array_slice,
                                      ") must be 0 for non-array texture 1D/2D views");

        if(view_desc.num_array_slices != TextureViewDesc::s_remaining_array_slices
           && view_desc.num_array_slices > 1)
            TEX_VIEW_VALIDATION_ERROR("Number of slices in the view (", view_desc.num_array_slices,
                                      ") must be 1 (or 0) for non-array texture 1D/2D views");
    }
    else if(view_desc.texture_dim == RESOURCE_DIM_TEX_1D_ARRAY
            || view_desc.texture_dim == RESOURCE_DIM_TEX_2D_ARRAY
            || view_desc.texture_dim == RESOURCE_DIM_TEX_CUBE
            || view_desc.texture_dim == RESOURCE_DIM_TEX_CUBE_ARRAY)
    {
        if(view_desc.first_array_slice >= this->m_desc.array_size)
            TEX_VIEW_VALIDATION_ERROR("First array slice (", view_desc.first_array_slice,
                                      ") exceeds the number of slices in the texture array (",
                                      this->m_desc.array_size, ")");

        if(view_desc.num_array_slices != TextureViewDesc::s_remaining_array_slices
           && view_desc.first_array_slice + view_desc.num_array_slices > this->m_desc.array_size)
            TEX_VIEW_VALIDATION_ERROR("First slice (", view_desc.first_array_slice,
                                      ") and number of slices in the view (", view_desc.num_array_slices,
                                      ") specify more slices than target texture has (",
                                      this->m_desc.array_size, ")");
    }
    else if(view_desc.texture_dim == RESOURCE_DIM_TEX_3D)
    {
        auto mip_depth = this->m_desc.depth >> view_desc.most_detailed_mip;
        if(view_desc.first_depth_slice + view_desc.num_depth_slices > mip_depth)
            TEX_VIEW_VALIDATION_ERROR("First slice (", view_desc.first_depth_slice,
                                      ") and number of slices in the view (", view_desc.num_depth_slices,
                                      ") specify more slices than target 3D texture mip level has (",
                                      mip_depth, ")");
    }
    else
    {
        UNEXPECTED("Unexpected texture dimension");
    }

    if(GetTextureFormatInfo(view_desc.format).is_typeless)
    {
        TEX_VIEW_VALIDATION_ERROR("Texture view format (", GetTextureFormatInfo(view_desc.format).name,
                                  ") cannot be typeless");
    }

#undef TEX_VIEW_VALIDATION_ERROR

    if(view_desc.num_mip_levels == 0 || view_desc.num_mip_levels == TextureViewDesc::s_remaining_mip_levels)
    {
        if(view_desc.view_type == TEXTURE_VIEW_SHADER_RESOURCE)
            view_desc.num_mip_levels = this->m_desc.mip_levels - view_desc.most_detailed_mip;
        else
            view_desc.num_mip_levels = 1;
    }

    if(view_desc.num_array_slices == 0
       || view_desc.num_array_slices == TextureViewDesc::s_remaining_array_slices)
    {
        if(view_desc.texture_dim == RESOURCE_DIM_TEX_1D_ARRAY
           || view_desc.texture_dim == RESOURCE_DIM_TEX_2D_ARRAY
           || view_desc.texture_dim == RESOURCE_DIM_TEX_CUBE
           || view_desc.texture_dim == RESOURCE_DIM_TEX_CUBE_ARRAY)
            view_desc.num_array_slices = this->m_desc.array_size - view_desc.first_array_slice;
        else if(view_desc.texture_dim == RESOURCE_DIM_TEX_3D)
        {
            auto mip_depth             = this->m_desc.depth >> view_desc.most_detailed_mip;
            view_desc.num_depth_slices = mip_depth - view_desc.first_depth_slice;
        }
        else
            view_desc.num_array_slices = 1;
    }

    if((view_desc.view_type == TEXTURE_VIEW_RENDER_TARGET)
       && (view_desc.format == TEX_FORMAT_R8_SNORM || view_desc.format == TEX_FORMAT_RG8_SNORM
           || view_desc.format == TEX_FORMAT_RGBA8_SNORM || view_desc.format == TEX_FORMAT_R16_SNORM
           || view_desc.format == TEX_FORMAT_RG16_SNORM || view_desc.format == TEX_FORMAT_RGBA16_SNORM))
    {
        const auto         fmt_name = GetTextureFormatInfo(view_desc.format).name;
        std::ostringstream ss;
        ss << fmt_name
           << " render target view is created.\n There might be an issue in OpenGL driver on NVidia "
              "hardware: when rendering to SNORM textures, all negative values are clamped to zero. Use "
              "UNORM format instead."
           << std::endl;
        Log::Log(Log::warning, ss.str());
    }
}

template<typename BaseInterface, typename TRenderDeviceImpl, typename TTextureViewImpl>
void TextureBase<BaseInterface, TRenderDeviceImpl, TTextureViewImpl>::createDefaultViews()
{
    const auto & tex_fmt_info = GetTextureFormatInfo(this->m_desc.format);
    if(tex_fmt_info.component_type == COMPONENT_TYPE_UNDEFINED)
    {
        // Cannot create default view for TYPELESS formats
        return;
    }

    if(this->m_desc.bind_flags & BIND_SHADER_RESOURCE)
    {
        TextureViewDesc view_desc;
        view_desc.view_type  = TEXTURE_VIEW_SHADER_RESOURCE;
        ITextureView * p_srv = nullptr;
        createViewInternal(view_desc, &p_srv, true);
        mup_default_srv.reset(static_cast<TTextureViewImpl *>(p_srv));
        assert(mup_default_srv->getDesc().view_type
               == TEXTURE_VIEW_SHADER_RESOURCE);   // "Unexpected view type"
    }

    if(this->m_desc.bind_flags & BIND_RENDER_TARGET)
    {
        TextureViewDesc view_desc;
        view_desc.view_type  = TEXTURE_VIEW_RENDER_TARGET;
        ITextureView * p_rtv = nullptr;
        createViewInternal(view_desc, &p_rtv, true);
        mup_default_rtv.reset(static_cast<TTextureViewImpl *>(p_rtv));
        assert(mup_default_rtv->getDesc().view_type == TEXTURE_VIEW_RENDER_TARGET);   //"Unexpected view type"
    }

    if(this->m_desc.bind_flags & BIND_DEPTH_STENCIL)
    {
        TextureViewDesc view_desc;
        view_desc.view_type  = TEXTURE_VIEW_DEPTH_STENCIL;
        ITextureView * p_dsv = nullptr;
        createViewInternal(view_desc, &p_dsv, true);
        mup_default_dsv.reset(static_cast<TTextureViewImpl *>(p_dsv));
        assert(mup_default_dsv->getDesc().view_type == TEXTURE_VIEW_DEPTH_STENCIL);   //"Unexpected view type"
    }

    if(this->m_desc.bind_flags & BIND_UNORDERED_ACCESS)
    {
        TextureViewDesc view_desc;
        view_desc.view_type    = TEXTURE_VIEW_UNORDERED_ACCESS;
        view_desc.access_flags = UAV_ACCESS_FLAG_READ_WRITE;
        ITextureView * p_uav   = nullptr;
        createViewInternal(view_desc, &p_uav, true);
        mup_default_uav.reset(static_cast<TTextureViewImpl *>(p_uav));
        assert(mup_default_uav->getDesc().view_type
               == TEXTURE_VIEW_UNORDERED_ACCESS);   //"Unexpected view type"
    }
}
}   // namespace evnt
#endif   // TEXTUREBASE_H
