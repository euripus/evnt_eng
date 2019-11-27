#include "glcontextstate.h"

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
std::string LogError(const char * Function, const char * FullFilePath, int Line, const ArgsType &... Args)
{
    std::string FileName(FullFilePath);
    auto        LastSlashPos = FileName.find_last_of("/\\");
    if(LastSlashPos != std::string::npos)
        FileName.erase(0, LastSlashPos + 1);
    auto Msg = Diligent::FormatString(Args...);
    // No callback set - output to cerr
    std::cerr << "Diligent Engine: " << (bThrowException ? "Fatal Error" : "Error") << " in " << Function
              << "() (" << FileName << ", " << Line << "): " << Msg << '\n';
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
        // CHECK_GL_ERROR("Failed to set GL program");
    }
}
}   // namespace evnt
