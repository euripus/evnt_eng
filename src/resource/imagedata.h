#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "../fs/file_system.h"
#include <cstdint>
#include <memory>
#include <string>

namespace evnt
{
struct ImageData
{
    // origin is the lower-left corner
    enum class PixelType
    {
        pt_rgb,
        pt_rgba,
        pt_none
    };

    uint32_t                   width;
    uint32_t                   height;
    PixelType                  type;
    std::unique_ptr<uint8_t[]> data;
};

bool ReadBMP(FileSystem::FilePtr file, ImageData & id);
bool ReadTGA(FileSystem::FilePtr file, ImageData & id);

FileSystem::MemoryFilePtr WriteTGA(std::string fname, const ImageData & id);
}   // namespace evnt
#endif   // IMAGEDATA_H
