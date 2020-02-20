#include "mousecursor.h"
#include "../core/core.h"
#include <cstring>

namespace evnt
{
MouseCursor::MouseCursor()
{
    m_hot_spot_x = 0;
    m_hot_spot_y = 0;

    m_shape       = StdCursorShape::ARROW;
    m_is_standart = true;

    m_ref_texture.height = 16;
    m_ref_texture.width  = 16;
    m_ref_texture.type   = ImageData::PixelType::pt_rgba;

    auto buffer = std::make_unique<uint8_t[]>(16 * 16 * 4);
    std::memset(buffer.get(), 0xff, 16 * 16 * 4);
    m_ref_texture.data = std::move(buffer);
}

MouseCursor::~MouseCursor() {}

MouseCursor::MouseCursor(const MouseCursor & rhs)
{
    m_filename    = rhs.m_filename;
    m_hot_spot_x  = rhs.m_hot_spot_x;
    m_hot_spot_y  = rhs.m_hot_spot_y;
    m_shape       = rhs.m_shape;
    m_is_standart = rhs.m_is_standart;

    m_ref_texture.height = rhs.m_ref_texture.height;
    m_ref_texture.width  = rhs.m_ref_texture.width;
    m_ref_texture.type   = rhs.m_ref_texture.type;

    uint32_t bytes    = m_ref_texture.type == ImageData::PixelType::pt_rgba ? 4 : 3;
    uint32_t buf_size = m_ref_texture.height * m_ref_texture.width * bytes;

    auto buffer = std::make_unique<uint8_t[]>(buf_size);
    std::memcpy(buffer.get(), rhs.m_ref_texture.data.get(), buf_size);
    m_ref_texture.data = std::move(buffer);
}

MouseCursor & MouseCursor::operator=(const MouseCursor & rhs)
{
    m_filename    = rhs.m_filename;
    m_hot_spot_x  = rhs.m_hot_spot_x;
    m_hot_spot_y  = rhs.m_hot_spot_y;
    m_shape       = rhs.m_shape;
    m_is_standart = rhs.m_is_standart;

    m_ref_texture.height = rhs.m_ref_texture.height;
    m_ref_texture.width  = rhs.m_ref_texture.width;
    m_ref_texture.type   = rhs.m_ref_texture.type;

    uint32_t bytes    = m_ref_texture.type == ImageData::PixelType::pt_rgba ? 4 : 3;
    uint32_t buf_size = m_ref_texture.height * m_ref_texture.width * bytes;

    auto buffer = std::make_unique<uint8_t[]>(buf_size);
    std::memcpy(buffer.get(), rhs.m_ref_texture.data.get(), buf_size);
    m_ref_texture.data = std::move(buffer);

    return *this;
}

bool MouseCursor::load()
{
    if(Core::instance().getFileSystem().isExist(m_filename))
    {
        auto cur_file = Core::instance().getFileSystem().getFile(m_filename);
        m_is_standart = !ReadTGA(&cur_file, m_ref_texture);
        return !m_is_standart;
    }

    return false;
}

MouseCursor::StdCursorShape MouseCursor::GetShape(std::string const & shape_name)
{
    // https://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case
    MouseCursor::StdCursorShape ret           = MouseCursor::StdCursorShape::ARROW;
    std::string                 up_shape_name = shape_name;
    std::transform(up_shape_name.begin(), up_shape_name.end(), up_shape_name.begin(),
                   [](unsigned char c) { return std::toupper(c); });

    if(up_shape_name == "ARROW")
        ret = MouseCursor::StdCursorShape::ARROW;
    else if(up_shape_name == "IBEAM")
        ret = MouseCursor::StdCursorShape::IBEAM;
    else if(up_shape_name == "CROSSHAIR")
        ret = MouseCursor::StdCursorShape::CROSSHAIR;
    else if(up_shape_name == "HAND")
        ret = MouseCursor::StdCursorShape::HAND;
    else if(up_shape_name == "HRESIZE")
        ret = MouseCursor::StdCursorShape::HRESIZE;
    else if(up_shape_name == "VRESIZE")
        ret = MouseCursor::StdCursorShape::VRESIZE;

    return ret;
}
}   // namespace evnt
