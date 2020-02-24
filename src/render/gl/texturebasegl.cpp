#include "texturebasegl.h"
#include "../../core/exception.h"
#include "glcontextstate.h"
#include "gldebug.h"
#include "typeconversions.h"

namespace evnt
{
//TextureBaseGL::TextureBaseGL(RenderDeviceGLImpl * p_device_gl, const TextureDesc & tex_desc,
//                             GLenum bind_target, const TextureData * p_init_data) :
//    TTextureBase(p_device_gl, tex_desc),
//    m_gl_texture{true},   // Create Texture immediately
//    m_bind_target{bind_target},
//    m_gl_tex_format{TexFormatToGLInternalTexFormat(m_desc.format, m_desc.bind_flags)}
//{
//    assert(m_gl_tex_format != 0);   //"Unsupported texture format"

//    if(tex_desc.usage == USAGE_STATIC && p_init_data == nullptr)
//    {
//        std::ostringstream ss;
//        ss << "Static Texture must be initialized with data at creation time" << std::endl;
//        Log::Log(Log::error, ss.str());
//        EV_EXCEPT(ss.str());
//    }
//}

//void TextureBaseGL::createViewInternal(const struct TextureViewDesc &  org_view_desc,
//                                       std::shared_ptr<ITextureView> & pp_view, bool is_default_view)
//{
//    assert(pp_view != nullptr);   // Null pointer provided
//    if(!pp_view)
//        return;
//    assert(pp_view);   // Overwriting reference to existing object may cause memory leaks"
//    //*pp_view = nullptr;

//    try
//    {
//        auto ViewDesc = org_view_desc;
//        correctTextureViewDesc(ViewDesc);

//        auto * pDeviceGLImpl = static_cast<RenderDeviceGLImpl *>(getDevice());

//        // http://www.opengl.org/wiki/Texture_Storage#Texture_views
//        GLenum GLViewFormat = TexFormatToGLInternalTexFormat(ViewDesc.format, m_desc.bind_flags);
//        assert(GLViewFormat != 0);   // Unsupported texture format

//        std::shared_ptr<TextureViewGLImpl> pViewOGL{nullptr};
//        if(ViewDesc.view_type == TEXTURE_VIEW_SHADER_RESOURCE)
//        {
//            bool bIsFullTextureView =
//                ViewDesc.texture_dim == m_desc.type
//                && ViewDesc.format
//                       == GetDefaultTextureViewFormat(m_desc.format, ViewDesc.view_type, m_desc.bind_flags)
//                && ViewDesc.most_detailed_mip == 0 && ViewDesc.num_mip_levels == m_desc.mip_levels
//                && ViewDesc.first_array_slice == 0 && ViewDesc.num_array_slices == m_desc.array_size;

//            pViewOGL = std::make_shared<TextureViewGLImpl>();
//            /*is_default_view ? this : nullptr)(
//            pDeviceGLImpl, ViewDesc, this,
//            !bIsFullTextureView,   // Create OpenGL texture view object if view
//            // does not address the whole texture
//            is_default_view);*/

//            if(!bIsFullTextureView)
//            {
//                GLenum GLViewTarget = 0;
//                GLuint NumLayers    = ViewDesc.num_array_slices;
//                switch(ViewDesc.texture_dim)
//                {
//                    case RESOURCE_DIM_TEX_1D:
//                        GLViewTarget              = GL_TEXTURE_1D;
//                        ViewDesc.num_array_slices = NumLayers = 1;
//                        break;

//                    case RESOURCE_DIM_TEX_1D_ARRAY:
//                        GLViewTarget = GL_TEXTURE_1D_ARRAY;
//                        break;

//                    case RESOURCE_DIM_TEX_2D:
//                        GLViewTarget = m_desc.sample_count > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
//                        ViewDesc.num_array_slices = NumLayers = 1;
//                        break;

//                    case RESOURCE_DIM_TEX_2D_ARRAY:
//                        GLViewTarget =
//                            m_desc.sample_count > 1 ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_ARRAY;
//                        break;

//                    case RESOURCE_DIM_TEX_3D: {
//                        GLViewTarget = GL_TEXTURE_3D;
//                        // If target is GL_TEXTURE_3D, NumLayers must equal 1.
//                        uint32_t MipDepth = std::max(m_desc.depth >> ViewDesc.most_detailed_mip, 1U);
//                        if(ViewDesc.first_depth_slice != 0 || ViewDesc.num_depth_slices != MipDepth)
//                        {
//                            std::ostringstream ss;
//                            ss << "3D texture view '" << ViewDesc.name
//                               << "' (most detailed mip: " << ViewDesc.most_detailed_mip
//                               << "; mip levels: " << ViewDesc.num_mip_levels
//                               << "; first slice: " << ViewDesc.first_depth_slice
//                               << "; num depth slices: " << ViewDesc.num_depth_slices << ") of texture '"
//                               << m_desc.name
//                               << "' does not references all depth slices. 3D texture views in OpenGL must "
//                                  "address all depth slices."
//                               << std::endl;
//                            Log::Log(Log::error, ss.str());

//                            ViewDesc.num_depth_slices  = MipDepth;
//                            ViewDesc.first_depth_slice = 0;
//                        }
//                        NumLayers = 1;
//                        break;
//                    }

//                    case RESOURCE_DIM_TEX_CUBE:
//                        GLViewTarget = GL_TEXTURE_CUBE_MAP;
//                        break;

//                    case RESOURCE_DIM_TEX_CUBE_ARRAY:
//                        GLViewTarget = GL_TEXTURE_CUBE_MAP_ARRAY;
//                        break;

//                    default:
//                        UNEXPECTED("Unsupported texture view type");
//                }

//                glTextureView(pViewOGL->getHandle(), GLViewTarget, m_gl_texture, GLViewFormat,
//                              ViewDesc.most_detailed_mip, ViewDesc.num_mip_levels, ViewDesc.first_array_slice,
//                              NumLayers);
//                CHECK_GL_ERROR_AND_THROW("Failed to create texture view");
//                pViewOGL->setBindTarget(GLViewTarget);
//            }
//        }
//        else if(ViewDesc.view_type == TEXTURE_VIEW_UNORDERED_ACCESS)
//        {
//            assert(
//                ViewDesc.num_array_slices == 1
//                || m_desc.type == RESOURCE_DIM_TEX_3D
//                       && ViewDesc.num_depth_slices
//                              == std::max(m_desc.depth >> ViewDesc.most_detailed_mip, 1U)
//                || ViewDesc.num_array_slices
//                       == m_desc
//                              .array_size);   //                   "Only single array/depth slice or the whole
//                                              //                   texture can be bound as UAV in OpenGL."
//            assert(ViewDesc.access_flags != 0);   // "At least one access flag must be specified"
//            pViewOGL = NEW_RC_OBJ(TexViewAllocator, "TextureViewGLImpl instance", TextureViewGLImpl,
//                                  is_default_view ? this : nullptr)(
//                pDeviceGLImpl, ViewDesc, this,
//                false,   // Do NOT create texture view OpenGL object
//                is_default_view);
//        }
//        else if(ViewDesc.view_type == TEXTURE_VIEW_RENDER_TARGET)
//        {
//            assert(ViewDesc.num_mip_levels == 1);   // "Only single mip level can be bound as RTV"
//            pViewOGL = NEW_RC_OBJ(TexViewAllocator, "TextureViewGLImpl instance", TextureViewGLImpl,
//                                  is_default_view ? this : nullptr)(
//                pDeviceGLImpl, ViewDesc, this,
//                false,   // Do NOT create texture view OpenGL object
//                is_default_view);
//        }
//        else if(ViewDesc.view_type == TEXTURE_VIEW_DEPTH_STENCIL)
//        {
//            assert(ViewDesc.num_mip_levels == 1);   // "Only single mip level can be bound as DSV"
//            pViewOGL = NEW_RC_OBJ(TexViewAllocator, "TextureViewGLImpl instance", TextureViewGLImpl,
//                                  is_default_view ? this : nullptr)(
//                pDeviceGLImpl, ViewDesc, this,
//                false,   // Do NOT create texture view OpenGL object
//                is_default_view);
//        }

//        if(is_default_view)
//            pp_view = pViewOGL;
//    }
//    catch(const Exception &)
//    {
//        const auto *       view_type_name = GetTexViewTypeLiteralName(org_view_desc.view_type);
//        std::ostringstream ss;
//        ss << "Failed to create view \"" << org_view_desc.name << "\" (" << view_type_name
//           << ") for texture \"" << m_desc.name << "\"" << std::endl;
//        Log::Log(Log::error, ss.str());
//    }
//}
}   // namespace evnt
