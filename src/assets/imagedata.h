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

bool ReadBMP(BaseFile const * file, ImageData & id);
bool ReadTGA(BaseFile const * file, ImageData & id);

OutFile WriteTGA(std::string fname, ImageData const & id);
}   // namespace evnt
#endif   // IMAGEDATA_H
