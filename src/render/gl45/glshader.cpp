#include "glshader.h"
#include "../../log/log.h"
#include <GL/glew.h>
#include <sstream>

namespace evnt
{
GLShader::GLShader(std::string src, std::set<std::string> defines)
{
    m_id                                          = glCreateProgram();
    const std::pair<GLenum, std::string> stages[] = {{GL_VERTEX_SHADER, "VERTEX"},
                                                     {GL_FRAGMENT_SHADER, "FRAGMENT"},
                                                     {GL_GEOMETRY_SHADER, "GEOMETRY"},
                                                     {GL_COMPUTE_SHADER, "COMPUTE"},
                                                     {GL_TESS_CONTROL_SHADER, "TESSCONTROL"},
                                                     {GL_TESS_EVALUATION_SHADER, "TESSEVAL"}};

    std::string define_string;
    for(std::string define : defines)
        define_string += "\n#define " + define;

    std::vector<GLuint> shaders;
    m_compile_success = true;
    for(int i = 0; i != 6; ++i)
    {
        if(src.find(stages[i].second) != std::string::npos)
        {
            GLuint shader = glCreateShader(stages[i].first);
            shaders.push_back(shader);

            std::string               define = "#version 460\n#define " + stages[i].second;
            std::vector<const char *> lines  = {define.data(), define_string.c_str(), "\n#line 0\n",
                                               src.data()};

            glShaderSource(shader, 4, lines.data(), nullptr);
            glCompileShader(shader);
            // DO ERROR CHECKING

            GLsizei len;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
            std::vector<char> str(len + 1);
            str[len] = '\0';
            glGetShaderInfoLog(shader, len, &len, str.data());
            glAttachShader(m_id, shader);
            m_shader_info_logs.push_back(stages[i].second + " INFO LOG: \n" + std::string(str.data()));

            GLint status;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if(status != GL_TRUE)
            {
                m_compile_success = false;

                std::ostringstream ss;
                ss << stages[i].second << " "
                   << " COMPILATION FAILED! " << str.data();
                Log::Log(Log::error, ss.str());
            }
        }
        else
            m_shader_info_logs.push_back("SHADER STAGE " + stages[i].second + " NOT AVAILABLE");
    }
    glLinkProgram(m_id);
    for(GLuint shader : shaders)
        glDeleteShader(shader);

    // DO ERROR CHECKING

    GLsizei len;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);
    std::vector<char> str(len + 1);
    str[len] = '\0';
    glGetProgramInfoLog(m_id, len, &len, str.data());
    m_info_log = std::string(str.data());

    GLint status;
    glGetProgramiv(m_id, GL_LINK_STATUS, &status);
    if(status != GL_TRUE)
    {
        m_compile_success = false;

        std::ostringstream ss;
        ss << "PROGRAM LINK FAILED! " << m_info_log;
        Log::Log(Log::error, ss.str());
    }
}

GLShader::~GLShader()
{
    destroy();
}

void GLShader::destroy()
{
    if(m_id != 0)
        glDeleteProgram(m_id);

    m_id = 0;
}
}   // namespace evnt
