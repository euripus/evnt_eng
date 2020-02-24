#include "renderdeviceglimpl.h"
#include "glcontextstate.h"
#include "gldebug.h"
#include "globjwrapper.h"
#include "typeconversions.h"

namespace evnt
{
//bool RenderDeviceGLImpl::checkExtension(const char * extension_string)
//{
//    return m_extension_strings.find(extension_string) != m_extension_strings.end();
//}

//void RenderDeviceGLImpl::flagSupportedTexFormats()
//{
//    const auto & device_caps      = getDeviceCaps();
//    bool         is_gl33_or_above = device_caps.dev_type == DeviceType::OpenGL
//                            && (device_caps.major_version >= 4
//                                || (device_caps.major_version == 3 && device_caps.minor_version >= 3));

//    bool is_rgtc       = checkExtension("GL_ARB_texture_compression_rgtc");
//    bool is_bptc       = checkExtension("GL_ARB_texture_compression_bptc");
//    bool is_s3tc       = checkExtension("GL_EXT_texture_compression_s3tc");
//    bool is_tex_norm16 = checkExtension("GL_EXT_texture_norm16");   // Only for ES3.1+

//#define FLAG_FORMAT(fmt, is_supported) m_texture_formats_info[fmt].supported = is_supported

//    // The formats marked by true below are required in GL 3.3+ and GLES 3.0+
//    // Note that GLES2.0 does not specify any required formats

//    FLAG_FORMAT(TEX_FORMAT_RGBA32_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA32_FLOAT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA32_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA32_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGB32_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_RGB32_FLOAT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGB32_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGB32_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA16_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA16_FLOAT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA16_UNORM, is_gl33_or_above || is_tex_norm16);
//    FLAG_FORMAT(TEX_FORMAT_RGBA16_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA16_SNORM, is_gl33_or_above || is_tex_norm16);
//    FLAG_FORMAT(TEX_FORMAT_RGBA16_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RG32_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_RG32_FLOAT, true);
//    FLAG_FORMAT(TEX_FORMAT_RG32_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RG32_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R32G8X24_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_D32_FLOAT_S8X24_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_X32_TYPELESS_G8X24_UINT, false);
//    FLAG_FORMAT(TEX_FORMAT_RGB10A2_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_RGB10A2_UNORM, true);
//    FLAG_FORMAT(TEX_FORMAT_RGB10A2_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R11G11B10_FLOAT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA8_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA8_UNORM, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA8_UNORM_SRGB, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA8_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA8_SNORM, true);
//    FLAG_FORMAT(TEX_FORMAT_RGBA8_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RG16_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_RG16_FLOAT, true);
//    FLAG_FORMAT(TEX_FORMAT_RG16_UNORM, is_gl33_or_above || is_tex_norm16);
//    FLAG_FORMAT(TEX_FORMAT_RG16_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RG16_SNORM, is_gl33_or_above || is_tex_norm16);
//    FLAG_FORMAT(TEX_FORMAT_RG16_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R32_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_D32_FLOAT, true);
//    FLAG_FORMAT(TEX_FORMAT_R32_FLOAT, true);
//    FLAG_FORMAT(TEX_FORMAT_R32_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R32_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R24G8_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_D24_UNORM_S8_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R24_UNORM_X8_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_X24_TYPELESS_G8_UINT, false);
//    FLAG_FORMAT(TEX_FORMAT_RG8_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_RG8_UNORM, true);
//    FLAG_FORMAT(TEX_FORMAT_RG8_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_RG8_SNORM, true);
//    FLAG_FORMAT(TEX_FORMAT_RG8_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R16_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_R16_FLOAT, true);
//    FLAG_FORMAT(TEX_FORMAT_D16_UNORM, true);
//    FLAG_FORMAT(TEX_FORMAT_R16_UNORM, is_gl33_or_above || is_tex_norm16);
//    FLAG_FORMAT(TEX_FORMAT_R16_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R16_SNORM, is_gl33_or_above || is_tex_norm16);
//    FLAG_FORMAT(TEX_FORMAT_R16_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R8_TYPELESS, true);
//    FLAG_FORMAT(TEX_FORMAT_R8_UNORM, true);
//    FLAG_FORMAT(TEX_FORMAT_R8_UINT, true);
//    FLAG_FORMAT(TEX_FORMAT_R8_SNORM, true);
//    FLAG_FORMAT(TEX_FORMAT_R8_SINT, true);
//    FLAG_FORMAT(TEX_FORMAT_A8_UNORM, false);   // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_R1_UNORM, false);   // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_RGB9E5_SHAREDEXP, true);
//    FLAG_FORMAT(TEX_FORMAT_RG8_B8G8_UNORM, false);    // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_G8R8_G8B8_UNORM, false);   // Not supported in OpenGL

//    FLAG_FORMAT(TEX_FORMAT_BC1_TYPELESS, is_s3tc);
//    FLAG_FORMAT(TEX_FORMAT_BC1_UNORM, is_s3tc);
//    FLAG_FORMAT(TEX_FORMAT_BC1_UNORM_SRGB, is_s3tc);
//    FLAG_FORMAT(TEX_FORMAT_BC2_TYPELESS, is_s3tc);
//    FLAG_FORMAT(TEX_FORMAT_BC2_UNORM, is_s3tc);
//    FLAG_FORMAT(TEX_FORMAT_BC2_UNORM_SRGB, is_s3tc);
//    FLAG_FORMAT(TEX_FORMAT_BC3_TYPELESS, is_s3tc);
//    FLAG_FORMAT(TEX_FORMAT_BC3_UNORM, is_s3tc);
//    FLAG_FORMAT(TEX_FORMAT_BC3_UNORM_SRGB, is_s3tc);

//    FLAG_FORMAT(TEX_FORMAT_BC4_TYPELESS, is_rgtc);
//    FLAG_FORMAT(TEX_FORMAT_BC4_UNORM, is_rgtc);
//    FLAG_FORMAT(TEX_FORMAT_BC4_SNORM, is_rgtc);
//    FLAG_FORMAT(TEX_FORMAT_BC5_TYPELESS, is_rgtc);
//    FLAG_FORMAT(TEX_FORMAT_BC5_UNORM, is_rgtc);
//    FLAG_FORMAT(TEX_FORMAT_BC5_SNORM, is_rgtc);

//    FLAG_FORMAT(TEX_FORMAT_B5G6R5_UNORM, false);                 // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_B5G5R5A1_UNORM, false);               // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_BGRA8_UNORM, false);                  // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_BGRX8_UNORM, false);                  // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM, false);   // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_BGRA8_TYPELESS, false);               // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_BGRA8_UNORM_SRGB, false);             // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_BGRX8_TYPELESS, false);               // Not supported in OpenGL
//    FLAG_FORMAT(TEX_FORMAT_BGRX8_UNORM_SRGB, false);             // Not supported in OpenGL

//    FLAG_FORMAT(TEX_FORMAT_BC6H_TYPELESS, is_bptc);
//    FLAG_FORMAT(TEX_FORMAT_BC6H_UF16, is_bptc);
//    FLAG_FORMAT(TEX_FORMAT_BC6H_SF16, is_bptc);
//    FLAG_FORMAT(TEX_FORMAT_BC7_TYPELESS, is_bptc);
//    FLAG_FORMAT(TEX_FORMAT_BC7_UNORM, is_bptc);
//    FLAG_FORMAT(TEX_FORMAT_BC7_UNORM_SRGB, is_bptc);

//#ifdef DEBUG
//    bool is_gl43_or_above = device_caps.dev_type == DeviceType::OpenGL
//                            && (device_caps.major_version >= 5
//                                || (device_caps.major_version == 4 && device_caps.minor_version >= 3));

//    constexpr int        test_texture_dim = 8;
//    constexpr int        max_texel_size   = 16;
//    std::vector<uint8_t> zero_data(test_texture_dim * test_texture_dim * max_texel_size);

//    // Go through all formats and try to create small 2D texture to check if the format is supported
//    for(auto fmt_info = m_texture_formats_info.begin(); fmt_info != m_texture_formats_info.end(); ++fmt_info)
//    {
//        if(fmt_info->format == TEX_FORMAT_UNKNOWN)
//            continue;

//        auto gl_fmt = TexFormatToGLInternalTexFormat(fmt_info->format);
//        if(gl_fmt == 0)
//        {
//            assert(!fmt_info->supported);   // "Format should be marked as unsupported"
//            continue;
//        }

//#if GL_ARB_internalformat_query2
//        // Only works on GL4.3+
//        if(is_gl43_or_above)
//        {
//            GLint params = 0;
//            glGetInternalformativ(GL_TEXTURE_2D, gl_fmt, GL_INTERNALFORMAT_SUPPORTED, 1, &params);
//            CHECK_GL_ERROR("glGetInternalformativ() failed");
//            assert(fmt_info->supported == (params == GL_TRUE));   //"This internal format should be supported"
//        }
//#endif

//        // Check that the format is indeed supported
//        if(fmt_info->supported)
//        {
//            GLObjectWrappers::GLTextureObj test_gl_tex(true);
//            // Immediate context has not been created yet, so use raw GL functions
//            glBindTexture(GL_TEXTURE_2D, test_gl_tex);
//            CHECK_GL_ERROR("Failed to bind texture");
//            glTexStorage2D(GL_TEXTURE_2D, 1, gl_fmt, test_texture_dim, test_texture_dim);
//            if(glGetError() == GL_NO_ERROR)
//            {
//                // It turned out it is not enough to only allocate texture storage
//                // For some reason glTexStorage2D() may succeed, but upload operation
//                // will later fail. So we need to additionally try to upload some
//                // data to the texture
//                const auto & transfer_attribs = GetNativePixelTransferAttribs(fmt_info->format);
//                if(transfer_attribs.is_compressed)
//                {
//                    const auto & fmt_attribs = GetTextureFormatInfo(fmt_info->format);
//                    static_assert((test_texture_dim & (test_texture_dim - 1)) == 0,
//                                  "Test texture dim must be power of two!");
//                    auto block_bytes_in_row =
//                        (test_texture_dim / int{fmt_attribs.block_width}) * int{fmt_attribs.component_size};
//                    glCompressedTexSubImage2D(GL_TEXTURE_2D, 0,   // mip level
//                                              0, 0, test_texture_dim, test_texture_dim, gl_fmt,
//                                              (test_texture_dim / int{fmt_attribs.block_height})
//                                                  * block_bytes_in_row,
//                                              zero_data.data());
//                }
//                else
//                {
//                    glTexSubImage2D(GL_TEXTURE_2D, 0,   // mip level
//                                    0, 0, test_texture_dim, test_texture_dim, transfer_attribs.pixel_format,
//                                    transfer_attribs.data_type, zero_data.data());
//                }

//                if(glGetError() != GL_NO_ERROR)
//                {
//                    Log::Log(Log::warning,
//                             Log::cstr_log(
//                                 "Failed to upload data to a test %dx%d %s texture.This likely indicates "
//                                 "that the format is not supported despite being reported so by the device.",
//                                 test_texture_dim, test_texture_dim, fmt_info->name.c_str()));
//                    fmt_info->supported = false;
//                }
//            }
//            else
//            {
//                Log::Log(Log::warning,
//                         Log::cstr_log(
//                             "Failed to allocate storage for a test %dx%d %s texture.This likely indicates "
//                             "that the format is not supported despite being reported so by the device.",
//                             test_texture_dim, test_texture_dim, fmt_info->name.c_str()));
//                fmt_info->supported = false;
//            }
//            glBindTexture(GL_TEXTURE_2D, 0);
//        }
//    }
//#endif
//}

//template<typename CreateFuncType>
//bool CreateTestGLTexture(GLContextState & gl_ctx_state, GLenum bind_target,
//                         const GLObjectWrappers::GLTextureObj & gl_tex_obj, CreateFuncType CreateFunc)
//{
//    gl_ctx_state.bindTexture(-1, bind_target, gl_tex_obj);
//    CreateFunc();
//    bool is_success = glGetError() == GL_NO_ERROR;
//    gl_ctx_state.bindTexture(-1, bind_target, GLObjectWrappers::GLTextureObj(false));
//    return is_success;
//}

//void RenderDeviceGLImpl::testTextureFormat(TEXTURE_FORMAT tex_format) {}
}   // namespace evnt
