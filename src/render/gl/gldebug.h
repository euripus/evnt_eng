#ifndef GLDEBUG_H
#define GLDEBUG_H

#include "../../log/log.h"
#include <cassert>
#include <sstream>

#ifdef DEBUG
#define CHECK_GL_ERROR(...)                                                                  \
    do                                                                                       \
    {                                                                                        \
        auto err = glGetError();                                                             \
        if(err != GL_NO_ERROR)                                                               \
        {                                                                                    \
            LogError(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__, "GL Error Code: ", err); \
        }                                                                                    \
    } while(false)
#else
#define CHECK_GL_ERROR(...)
#endif

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

}   // namespace evnt

#endif   // GLDEBUG_H
