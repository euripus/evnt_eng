#ifndef NULLSTREAMSOURCE_H
#define NULLSTREAMSOURCE_H

#include "assetmanager.h"

class NullStreamSource : public IAssetStreamIO
{
public:
    NullStreamSource() {}

    void    openStreamRead() override {}
    void    openStreamWrite(bool overwrite) override {}
    void    closeStream() override {}
    void    writeStreamBytes(char * buffer, uint32_t length) override {}
    void    readStreamBytes(char * buffer, uint32_t length) override {}
    bool    canRead() override { return false; }
    bool    canWrite() override { return false; }
    void    seekStream(uint32_t position) override {}
    int32_t getStreamPosition() override { return -1; }
    int32_t getStreamLength() override { return 0; }
};

#endif   // NULLSTREAMSOURCE_H
