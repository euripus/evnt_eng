#include "memoryassetstream.h"
#include <cassert>
#include <glm/glm.hpp>

MemoryAssetStream::MemoryAssetStream(std::vector<char> & memory) : m_mem(memory) {}

void MemoryAssetStream::openStreamRead() {}

void MemoryAssetStream::openStreamWrite(bool overwrite)
{
    if(overwrite)
        m_position = 0;
}

void MemoryAssetStream::closeStream() {}

void MemoryAssetStream::writeStreamBytes(char * buffer, uint32_t length)
{
    if(m_mem.size() - m_position - 1 < length)
        m_mem.reserve(m_mem.size() + length - m_position - 1);

    memcpy(m_mem.data(), buffer, glm::min<int>(length, m_mem.size() - m_position - 1));
}

void MemoryAssetStream::readStreamBytes(char * buffer, uint32_t length)
{
    memcpy(buffer, m_mem.data(), glm::min<int>(length, m_mem.size() - m_position - 1));
}

bool MemoryAssetStream::canRead()
{
    return true;
}

bool MemoryAssetStream::canWrite()
{
    return true;
}

void MemoryAssetStream::seekStream(uint32_t position)
{
    assert(position <= m_position);

    m_position = position;
}

int MemoryAssetStream::getStreamPosition()
{
    return m_position;
}

int MemoryAssetStream::getStreamLength()
{
    return m_mem.size();
}
