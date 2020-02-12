#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>

class IAssetStreamIO
{
public:
    virtual ~IAssetStreamIO() {}

    virtual void openStreamRead()                                 = 0;
    virtual void openStreamWrite(bool overwrite)                  = 0;
    virtual void closeStream()                                    = 0;
    virtual void writeStreamBytes(char * buffer, uint32_t length) = 0;
    virtual void readStreamBytes(char * buffer, uint32_t length)  = 0;
    virtual bool canRead()                                        = 0;
    virtual bool canWrite()                                       = 0;
    virtual void seekStream(uint32_t position)                    = 0;
    virtual int  getStreamPosition()                              = 0;
    virtual int  getStreamLength()                                = 0;

    template<class T>
    void writeStream(T * buffer, uint32_t length)
    {
        writeStreamBytes(reinterpret_cast<char *>(buffer), length * sizeof(T));
    }
    void writeString(std::string str)
    {
        unsigned int len = static_cast<unsigned int>(str.length());
        writeStream(&len, 1);
        writeStream(const_cast<char *>(str.c_str()), len);
    }

    template<class T>
    void writeStream(T value)
    {
        writeStreamBytes(reinterpret_cast<char *>(&value), sizeof(T));
    }

    template<class T>
    void readStream(T * buffer, uint32_t length)
    {
        readStreamBytes(reinterpret_cast<char *>(buffer), length * sizeof(T));
    }

    std::string readString()
    {
        unsigned int size = 0;
        readStream(&size, 1);
        return readString(size);
    }

    std::string readString(unsigned int length)
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
