#include "glcontextstate.h"
#include "../../log/log.h"
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
std::string LogError(const char * function, const char * full_file_path, int line, const ArgsType &... args)
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

    return ss.str();
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
}   // namespace evnt
