#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <cassert>
#include <ctime>
#include <string>

class IAssetStreamIO
{
protected:
    std::time_t m_last_write_time = 0;

public:
    virtual ~IAssetStreamIO() {}

    virtual void    openStreamRead()                                 = 0;
    virtual void    openStreamWrite(bool overwrite)                  = 0;
    virtual void    closeStream()                                    = 0;
    virtual void    writeStreamBytes(char * buffer, uint32_t length) = 0;
    virtual void    readStreamBytes(char * buffer, uint32_t length)  = 0;
    virtual bool    canRead()                                        = 0;
    virtual bool    canWrite()                                       = 0;
    virtual void    seekStream(uint32_t position)                    = 0;
    virtual int32_t getStreamPosition()                              = 0;
    virtual int32_t getStreamLength()                                = 0;

    std::time_t getTimeStamp() { return m_last_write_time; }

    template<class T>
    void writeStream(T * buffer, uint32_t length)
    {
        assert(canWrite());
        writeStreamBytes(reinterpret_cast<char *>(buffer), length * sizeof(T));
    }

    void writeString(std::string str)
    {
        uint32_t len = static_cast<uint32_t>(str.length());
        writeStream(len);
        writeStream(const_cast<char *>(str.c_str()), len);
    }

    template<class T>
    void writeStream(T value)
    {
        assert(canWrite());
        writeStreamBytes(reinterpret_cast<char *>(&value), sizeof(T));
    }

    template<class T>
    void readStream(T * buffer, uint32_t length)
    {
        assert(canRead());
        readStreamBytes(reinterpret_cast<char *>(buffer), length * sizeof(T));
    }

    std::string readString()
    {
        uint32_t size = 0;
        readStream(&size, 1);
        return readString(size);
    }

    std::string readString(uint32_t length)
    {
        char * buffer  = new char[length + 1];
        buffer[length] = '\0';
        readStream(buffer, length);
        std::string str = std::string(buffer);
        delete[] buffer;

        return str;
    }

    void seekStreamRelative(uint32_t delta) { seekStream(getStreamPosition() + delta); }
};

class AssetManager
{
public:
    AssetManager();
};

#endif   // ASSETMANAGER_H
