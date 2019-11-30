#ifndef DEBUG_MESSAGES_H
#define DEBUG_MESSAGES_H

#include "log.h"
#include <sstream>

#define UNEXPECTED(Message, ...)                                                                           \
    do                                                                                                     \
    {                                                                                                      \
        std::ostringstream ss;                                                                             \
        Log::stream_print(ss, Message, ##__VA_ARGS__);                                                     \
        std::string msg = ss.str();                                                                        \
        ss.str({});                                                                                        \
        ss << msg << " in function " << __FUNCTION__ << " in file " << __FILE__ << " in line " << __LINE__ \
           << std::endl;                                                                                   \
        Log::Log(Log::error, ss.str());                                                                    \
    } while(false)

#endif   // DEBUG_MESSAGES_H
