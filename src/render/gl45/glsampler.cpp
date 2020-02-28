#include "glsampler.h"
#include "typeconversions.h"

namespace evnt
{
GLSampler::GLSampler() : ISampler()
{
    glCreateSamplers(1, &m_id);
}

GLSampler::~GLSampler()
{
    destroy();
}

void GLSampler::destroy()
{
    if(m_id != 0)
        glDeleteSamplers(1, &m_id);

    m_id = 0;
}

int32_t GLSampler::bindToTextureUnit()
{
    int32_t unit = TextureUnitManager::RefreshUnit(m_id);
    glBindSampler(unit, m_id);

    return unit;
}

void GLSampler::setWrapMode(TextureChannel channel, WrapMode mode)
{
    glSamplerParameteri(m_id, TexChannelToGLTexChannel(channel), WrapModeToGLWrapMode(mode));
    switch(channel)
    {
        case TextureChannel::S:
            m_s_wrap = mode;
            break;
        case TextureChannel::T:
            m_t_wrap = mode;
            break;
        case TextureChannel::R:
            m_r_wrap = mode;
            break;
    }
}

void GLSampler::setMinificationFilter(MinificationFilter filter)
{
    glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, MinFilterToGLMinFilter(filter));
    m_min_filter = filter;
}

void GLSampler::setMagnificationFilter(MagnificationFilter filter)
{
    glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, MagFilterToGLMagFilter(filter));
    m_mag_filter = filter;
}

void GLSampler::setCompareMode(bool enabled)
{
    glSamplerParameteri(m_id, GL_TEXTURE_COMPARE_MODE, enabled ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE);
    m_depth_compare = enabled;
}

void GLSampler::setComparison(ComparisonFunc func)
{
    glSamplerParameteri(m_id, GL_TEXTURE_COMPARE_FUNC, CompareFuncToGLCompareFunc(func));
    m_compare_func = func;
}

void GLSampler::setAnisotropicFiltering(bool enabled)
{
    float aniso = 1.0f;
    if(enabled)
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
    glSamplerParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY, aniso);
    m_anisotropic_filtering = enabled;
}

void GLSampler::setBorderColor(glm::vec4 color)
{
    glSamplerParameterfv(m_id, GL_TEXTURE_BORDER_COLOR, reinterpret_cast<float *>(&color));
    m_border_color = color;
}
}   // namespace evnt
