#ifndef MEMORYASSETSTREAM_H
#define MEMORYASSETSTREAM_H

#include "assetmanager.h"
#include <vector>

class MemoryAssetStream : public IAssetStreamIO
{
public:
    MemoryAssetStream() = default;
    MemoryAssetStream(std::vector<char> & memory, std::time_t timestamp);

    void    openStreamRead() override;
    void    openStreamWrite(bool overwrite) override;
    void    closeStream() override;
    void    writeStreamBytes(char * buffer, uint32_t length) override;
    void    readStreamBytes(char * buffer, uint32_t length) override;
    bool    canRead() override;
    bool    canWrite() override;
    void    seekStream(uint32_t position) override;
    int32_t getStreamPosition() override;
    int32_t getStreamLength() override;

private:
    uint32_t          m_position = 0;
    std::vector<char> m_mem;
};

#endif   // MEMORYASSETSTREAM_H
