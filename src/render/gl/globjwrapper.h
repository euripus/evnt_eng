#ifndef GLOBJWRAPPER_H
#define GLOBJWRAPPER_H

#include <GL/glew.h>
#include <utility>

namespace GLObjectWrappers
{
template<typename GeneratorType>
class GLObjectWrapper
{
    GeneratorType m_gen;
    GLuint        m_gl_obj_id{0};

public:
    GLObjectWrapper(bool create)
    {
        if(create)
            m_gl_obj_id = m_gen.Create();
    }

    ~GLObjectWrapper()
    {
        if(m_gl_obj_id != 0)
        {
            m_gen.Release(m_gl_obj_id);
            m_gl_obj_id = 0;
        }
    }

    GLObjectWrapper(const GLObjectWrapper &) = delete;
    GLObjectWrapper & operator=(const GLObjectWrapper &) = delete;

    GLObjectWrapper & operator=(GLObjectWrapper && other)
    {
        std::swap(m_gl_obj_id, other.m_gl_obj_id);
        std::swap(m_gen, other.m_gen);
        return *this;
    }

    GLObjectWrapper(GLObjectWrapper && other)
    {
        m_gl_obj_id = other.m_gl_obj_id;
        m_gen       = other.m_gen;
    }

    operator GLuint() const { return m_gl_obj_id; }

    static GLObjectWrapper Null() { return GLObjectWrapper{false}; }
};

class TexGenerator
{
public:
    GLuint Create() const
    {
        GLuint new_tex{0};
        glGenTextures(1, &new_tex);

        return new_tex;
    }

    void Release(GLuint tex) { glDeleteTextures(1, &tex); }
};

class GLBufferObjGenerator
{
public:
    GLuint Create() const
    {
        GLuint new_buff_obj{0};
        glGenBuffers(1, &new_buff_obj);

        return new_buff_obj;
    }

    void Release(GLuint buff) { glDeleteBuffers(1, &buff); }
};

class GLProgramObjGenerator
{
public:
    GLuint Create() const { return glCreateProgram(); }
    void   Release(GLuint prg) { glDeleteProgram(prg); }
};

class GLShaderObjGenerator
{
public:
    GLShaderObjGenerator(GLenum ShaderType) : m_shader_type(ShaderType) {}
    GLuint Create() const { return glCreateShader(m_shader_type); }
    void   Release(GLuint shd) { glDeleteShader(shd); }

private:
    GLenum m_shader_type;
};

class GLPipelineObjGenerator
{
public:
    GLuint Create() const
    {
        GLuint new_pipeline{0};
        glGenProgramPipelines(1, &new_pipeline);

        return new_pipeline;
    }

    void Release(GLuint pipeline) { glDeleteProgramPipelines(1, &pipeline); }
};

class GLVertexArrayObjGenerator
{
public:
    GLuint Create() const
    {
        GLuint new_vao{0};
        glGenVertexArrays(1, &new_vao);

        return new_vao;
    }

    void Release(GLuint vao) { glDeleteVertexArrays(1, &vao); }
};

class GLSamplerObjGenerator
{
public:
    GLuint Create() const
    {
        GLuint new_smpl{0};
        glGenSamplers(1, &new_smpl);

        return new_smpl;
    }

    void Release(GLuint smpl) { glDeleteSamplers(1, &smpl); }
};

class GLFBOGenerator
{
public:
    GLuint Create() const
    {
        GLuint fbo{0};
        glGenFramebuffers(1, &fbo);

        return fbo;
    }

    void Release(GLuint fbo) { glDeleteFramebuffers(1, &fbo); }
};

class GLRBOGenerator
{
public:
    GLuint Create() const
    {
        GLuint rbo{0};
        glGenRenderbuffers(1, &rbo);

        return rbo;
    }

    void Release(GLuint rbo) { glDeleteRenderbuffers(1, &rbo); }
};

using GLTextureObj      = GLObjectWrapper<TexGenerator>;
using GLBufferObj       = GLObjectWrapper<GLBufferObjGenerator>;
using GLProgramObj      = GLObjectWrapper<GLProgramObjGenerator>;
using GLShaderObj       = GLObjectWrapper<GLShaderObjGenerator>;
using GLPipelineObj     = GLObjectWrapper<GLPipelineObjGenerator>;
using GLVertexArrayObj  = GLObjectWrapper<GLVertexArrayObjGenerator>;
using GLSamplerObj      = GLObjectWrapper<GLSamplerObjGenerator>;
using GLFrameBufferObj  = GLObjectWrapper<GLFBOGenerator>;
using GLRenderBufferObj = GLObjectWrapper<GLRBOGenerator>;
}   // namespace GLObjectWrappers

#endif   // GLOBJWRAPPER_H
