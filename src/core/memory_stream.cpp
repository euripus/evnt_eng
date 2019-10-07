#include "memory_stream.h"

#include <cstring>
#include <stdexcept>

namespace evnt
{
void OutputMemoryStream::write(const int8_t * inData, size_t inByteCount)
{
    m_buffer.reserve(m_buffer.size() + inByteCount);
    m_buffer.insert(std::end(m_buffer), inData, inData + inByteCount);
}

void OutputMemoryStream::write(const InputMemoryStream & inStream)
{
    m_buffer.reserve(m_buffer.size() + inStream.getCapacity());
    m_buffer.insert(std::end(m_buffer), inStream.getPtr(), inStream.getPtr() + inStream.getCapacity());
}

void InputMemoryStream::read(void * outData, uint32_t inByteCount) const
{
    uint32_t resultHead = m_head + inByteCount;
    if(resultHead > m_capacity)
    {
        throw std::range_error("InputMemoryStream::Read - no data to read!");
    }

    std::memcpy(outData, m_data.get() + m_head, inByteCount);

    m_head = resultHead;
}

InputMemoryStream InputMemoryStream::ConvertToInputMemoryStream(const OutputMemoryStream & inStream)
{
    std::unique_ptr<int8_t[]> new_buf(new int8_t[inStream.getLength()]);
    std::memcpy(new_buf.get(), inStream.getBufferPtr(), inStream.getLength());
    InputMemoryStream temp(std::move(new_buf), inStream.getLength());

    return temp;
}
}   // namespace evnt
