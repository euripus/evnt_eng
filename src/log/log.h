#ifndef ILOG_H
#define ILOG_H

#include <ostream>
#include <string>

/*
Log string sample:
LineID [Severity] [ThreadID] [TimeStamp] [Uptime] Message
*/
namespace evnt
{
namespace Log
{
    enum SeverityLevel
    {
        notification,
        warning,
        error
    };

    enum Output
    {
        all,
        console,
        file
    };

    namespace BoostLog
    {
        void InitBoostLog(std::string const & fname, Output out);

        std::string GetLogFileName();
        void        SetSeverityFilter(SeverityLevel sl);

        void Put(SeverityLevel sl, std::string const & message);
        void Put(SeverityLevel sl, std::wstring const & message);
    }   // namespace BoostLog

    void Log(SeverityLevel sl, std::wstring const & msg);
    void Log(SeverityLevel sl, std::string const & msg);

    // C-style printf fuction
    std::wstring cstr_log(wchar_t const * format, ...);
    std::string  cstr_log(char const * format, ...);

    // variadic print
    template<typename... Args>
    void stream_print(std::ostream & out, Args... args)
    {
        ((out << " " << args), ...) << std::endl;
    }

    SeverityLevel ConvertStrToSeverity(std::string sl_txt);
    Output        ConvertStrToOutput(std::string ot_txt);
}   // namespace Log
}   // namespace evnt

#define LOG_MESSAGE(message)   evnt::Log::BoostLog::put(evnt::Log::notification, std::string(message));
#define LOG_MESSAGE_W(message) evnt::Log::BoostLog::put(evnt::Log::notification, std::wstring(message));

#define LOG_WARNING(message)   evnt::Log::BoostLog::put(evnt::Log::warning, std::string(message));
#define LOG_WARNING_W(message) evnt::Log::BoostLog::put(evnt::Log::warning, std::wstring(message));

#define LOG_ERROR(message)   evnt::Log::BoostLog::put(evnt::Log::error, std::string(message));
#define LOG_ERROR_W(message) evnt::Log::BoostLog::put(evnt::Log::error, std::wstring(message));

#endif   // ILOG_H
