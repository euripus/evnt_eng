#include "texturebasegl.h"
#include "../../core/exception.h"
#include "typeconversions.h"

namespace evnt
{
TextureBaseGL::TextureBaseGL(RenderDeviceGLImpl * p_device_gl, const TextureDesc & tex_desc,
                             GLenum bind_target, const TextureData * p_init_data) :
    TTextureBase(p_device_gl, tex_desc),
    m_gl_texture{true},   // Create Texture immediately
    m_bind_target{bind_target},
    m_gl_tex_format{TexFormatToGLInternalTexFormat(m_desc.format, m_desc.bind_flags)}
{
    assert(m_gl_tex_format != 0);   //"Unsupported texture format"

    if(tex_desc.usage == USAGE_STATIC && p_init_data == nullptr)
    {
        std::ostringstream ss;
        ss << "Static Texture must be initialized with data at creation time" << std::endl;
        Log::Log(Log::error, ss.str());
        EV_EXCEPT(ss.str());
    }
}
}   // namespace evnt
