#include "file.h"
#include "../log/log.h"
#include "file_system.h"
#include <cassert>
#include <chrono>
#include <cstring>
#include <fstream>
#include <zlib.h>

//#include "zip.h"

namespace evnt
{
OutFile::OutFile()
{
    m_name            = FileSystem::GetTempFileName();
    m_last_write_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

OutFile::OutFile(std::string name)
{
    std::swap(m_name, name);
    m_last_write_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

OutFile::OutFile(std::string name, const char * data, size_t length) : OutFile(name)
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
