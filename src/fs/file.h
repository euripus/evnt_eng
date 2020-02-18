#ifndef FILE_H
#define FILE_H

#include "memory_stream.h"
#include <ctime>

namespace evnt
{
class IFile
{
public:
    virtual ~IFile() = default;

    virtual const int8_t * getData() const     = 0;
    virtual size_t         getFileSize() const = 0;
    virtual std::time_t    timeStamp() { return m_last_write_time; }
    virtual std::string    getName() const = 0;

protected:
    std::time_t m_last_write_time = 0;
    std::string m_name;
};

class OutFile : public IFile
{
public:
    OutFile();   // file for writing with random name
    OutFile(std::string name);
    OutFile(std::string name, const char * data, size_t length);
    ~OutFile() override = default;

    const int8_t * getData() const override { return m_data.getBufferPtr(); }
    size_t         getFileSize() const override { return m_data.getLength(); }
    std::string    getName() const override { return m_name; }

    void                 write(const char * buffer, size_t len);
    OutputMemoryStream & getStream() { return m_data; }

private:
    OutputMemoryStream m_data;
};

class InFile : public IFile
{
public:
    InFile(std::string name, std::time_t timestamp, size_t f_size, std::unique_ptr<int8_t[]> data) :
        m_data(std::move(data), f_size)
    {
        std::swap(m_name, name);
        m_last_write_time = timestamp;
    }
    ~InFile() override = default;

    const int8_t * getData() const override { return m_data.getPtr(); }
    size_t         getFileSize() const override { return m_data.getCapacity(); }
    std::string    getName() const override { return m_name; }

    InputMemoryStream & getStream() { return m_data; }

private:
    InputMemoryStream m_data;
};
}   // namespace evnt
#endif   // FILE_H
