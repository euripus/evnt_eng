#include "glcontextstate.h"
#include "../../log/log.h"
#include <cassert>
#include <sstream>

#define CHECK_GL_ERROR(...)                                                                  \
    do                                                                                       \
    {                                                                                        \
        auto err = glGetError();                                                             \
        if(err != GL_NO_ERROR)                                                               \
        {                                                                                    \
            LogError(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__, "GL Error Code: ", err); \
        }                                                                                    \
    } while(false)

namespace evnt
{
template<typename... ArgsType>
void LogError(const char * function, const char * full_file_path, int line, const ArgsType &... args)
{
    std::ostringstream ss;

    std::string file_name(full_file_path);
    auto        last_slash_pos = file_name.find_last_of("//");
    if(last_slash_pos != std::string::npos)
        file_name.erase(0, last_slash_pos + 1);

    Log::stream_print(ss, args...);
    std::string msg = ss.str();
    ss.str({});

    ss << "Error"
       << " in " << function << "() (" << file_name << ", " << line << "): " << msg << '\n';

    Log::Log(Log::error, ss.str());
}

GLContextState::GLContextState() {}

bool UpdateBoundObject(GLuint & current_object_id, GLuint & new_handle)
{
    if(current_object_id != new_handle)
    {
        current_object_id = new_handle;
        return true;
    }
    return false;
}

template<class ObjectType>
bool UpdateBoundObjectsArr(std::vector<ObjectType> & bound_object_ids, uint32_t index, GLuint & new_handle)
{
    if(index >= bound_object_ids.size())
        bound_object_ids.resize(index + 1, -1);

    return UpdateBoundObject(bound_object_ids[index], new_handle);
}

void GLContextState::setProgram(const GLObjectWrappers::GLProgramObj & program)
{
    GLuint prog_handle = program;
    if(UpdateBoundObject(m_prog_id, prog_handle))
    {
        glUseProgram(prog_handle);
        CHECK_GL_ERROR("Failed to set GL program");
    }
}

void GLContextState::setPipeline(const GLObjectWrappers::GLPipelineObj & pipeline)
{
    GLuint handle = pipeline;
    if(UpdateBoundObject(m_pipeline_id, handle))
    {
        glBindProgramPipeline(handle);
        CHECK_GL_ERROR("Failed to bind program pipeline");
    }
}

void GLContextState::bindVAO(const GLObjectWrappers::GLVertexArrayObj & vao)
{
    GLuint handle = vao;
    if(UpdateBoundObject(m_vao_id, handle))
    {
        glBindVertexArray(handle);
        CHECK_GL_ERROR("Failed to set VAO");
    }
}

void GLContextState::bindFBO(const GLObjectWrappers::GLFrameBufferObj & fbo)
{
    GLuint handle = fbo;
    if(UpdateBoundObject(m_fbo_id, handle))
    {
        // Even though the write mask only applies to writes to a framebuffer, the mask state is NOT
        // Framebuffer state. So it is NOT part of a Framebuffer Object or the Default Framebuffer.
        // Binding a new framebuffer will NOT affect the mask.

        // NOTE: if attachment image is a NON-immutable format texture and the selected
        // level is NOT level_base, the texture MUST BE MIPMAP COMPLETE
        // If image is part of a cubemap texture, the texture must also be mipmap cube complete.
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);
        CHECK_GL_ERROR("Failed to bind FBO as draw framebuffer");
        glBindFramebuffer(GL_READ_FRAMEBUFFER, handle);
        CHECK_GL_ERROR("Failed to bind FBO as read framebuffer");
    }
}

void GLContextState::setActiveTexture(int32_t index)
{
    if(index < 0)
    {
        index += m_caps.m_max_combined_tex_units;
    }
    assert(0 <= index && index < m_caps.m_max_combined_tex_units);

    if(m_active_texture != index)
    {
        glActiveTexture(GL_TEXTURE0 + index);
        CHECK_GL_ERROR("Failed to activate texture slot ", index);
        m_active_texture = index;
    }
}

void GLContextState::bindTexture(int32_t index, GLenum bind_target,
                                 const GLObjectWrappers::GLTextureObj & tex)
{
    if(index < 0)
    {
        index += m_caps.m_max_combined_tex_units;
    }
    assert(0 <= index && index < m_caps.m_max_combined_tex_units);

    // Always update active texture unit
    setActiveTexture(index);

    GLuint tex_handle = tex;
    if(UpdateBoundObjectsArr(m_bound_textures, index, tex_handle))
    {
        glBindTexture(bind_target, tex_handle);
        CHECK_GL_ERROR("Failed to bind texture to slot ", index);
    }
}

void GLContextState::bindSampler(uint32_t index, const GLObjectWrappers::GLSamplerObj & sampler)
{
    GLuint sampler_handle = sampler;
    if(UpdateBoundObjectsArr(m_bound_samplers, index, sampler_handle))
    {
        glBindSampler(index, sampler_handle);
        CHECK_GL_ERROR("Failed to bind sampler to slot ", index);
    }
}

void GLContextState::bindImage(uint32_t index, class TextureViewGLImpl * tex_view, GLint mip_level,
                               GLboolean is_layered, GLint layer, GLenum access, GLenum format)
{}

void GLContextState::ensureMemoryBarrier(uint32_t required_barriers, class AsyncWritableResource * res) {}

void GLContextState::setPendingMemoryBarriers(uint32_t pending_barriers)
{
    m_pending_memory_barriers |= pending_barriers;
}

void GLContextState::enableDepthTest(bool enable)
{
    if(m_ds_state.m_depth_enable_state != enable)
    {
        if(enable)
        {
            glEnable(GL_DEPTH_TEST);
            CHECK_GL_ERROR("Failed to enable detph test");
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
            CHECK_GL_ERROR("Failed to disable detph test");
        }
        m_ds_state.m_depth_enable_state = enable;
    }
}

void GLContextState::enableDepthWrites(bool enable)
{
    if(m_ds_state.m_depth_writes_enable_state != enable)
    {
        // If mask is non-zero, the depth buffer is enabled for writing; otherwise, it is disabled.
        glDepthMask(enable ? 1 : 0);
        CHECK_GL_ERROR("Failed to enale/disable depth writes");
        m_ds_state.m_depth_writes_enable_state = enable;
    }
}

void GLContextState::setDepthFunc(COMPARISON_FUNCTION cmp_func) {}

void GLContextState::enableStencilTest(bool enable) {}

void GLContextState::setStencilWriteMask(uint8_t stencil_writeMask) {}

void GLContextState::setStencilRef(GLenum face, int32_t ref) {}

void GLContextState::setStencilFunc(GLenum face, COMPARISON_FUNCTION func, int32_t ref, uint32_t mask) {}

void GLContextState::setStencilOp(GLenum face, STENCIL_OP stencil_fail_op, STENCIL_OP stencil_depth_fail_op,
                                  STENCIL_OP stencil_pass_op)
{}
}   // namespace evnt
