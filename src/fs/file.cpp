#include "file.h"
#include "../log/log.h"
#include "file_system.h"
#include <chrono>
#include <filesystem>

namespace evnt
{
std::string BaseFile::getNameExt() const
{
    //    auto n = m_name.rfind(".");
    //    if(n != std::string::npos)
    //    {
    //        return m_name.substr(n + 1);
    //    }
    //    else
    //        return {};

    return std::filesystem::path(m_name).extension();
}

OutFile::OutFile()
{
    m_name            = FileSystem::GetTempFileName();
    m_last_write_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

OutFile::OutFile(std::string name)
{
    m_name            = std::move(name);
    m_last_write_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

OutFile::OutFile(std::string name, const char * data, size_t length) : OutFile(std::move(name))
{
    m_data.write(reinterpret_cast<const int8_t *>(data), length);
}

OutFile::OutFile(const InFile & infile) :
    OutFile{infile.getName(), reinterpret_cast<const char *>(infile.getData()), infile.getFileSize()}
{}

void OutFile::write(const char * buffer, size_t len)
{
    assert(buffer != nullptr);
    assert(len > 0);

    m_data.write(reinterpret_cast<const int8_t *>(buffer), len);
    m_last_write_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

}   // namespace evnt
