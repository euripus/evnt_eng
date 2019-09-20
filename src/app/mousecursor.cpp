#include "mousecursor.h"
#include "../core/core.h"
#include <cstring>

namespace evnt
{
MouseCursor::MouseCursor()
{
    m_hotSpotX = 0;
    m_hotSpotY = 0;

    m_shape      = StdCursorShape::ARROW;
    m_isStandart = true;

    m_refTexture.height = 16;
    m_refTexture.width  = 16;
    m_refTexture.type   = ImageData::PixelType::pt_rgba;

    auto buffer = std::make_unique<uint8_t[]>(16 * 16 * 4);
    std::memset(buffer.get(), 0xff, 16 * 16 * 4);
    m_refTexture.data = std::move(buffer);
}

MouseCursor::~MouseCursor() {}

MouseCursor::MouseCursor(const MouseCursor & rhs)
{
    m_filename   = rhs.m_filename;
    m_hotSpotX   = rhs.m_hotSpotX;
    m_hotSpotY   = rhs.m_hotSpotY;
    m_shape      = rhs.m_shape;
    m_isStandart = rhs.m_isStandart;

    m_refTexture.height = rhs.m_refTexture.height;
    m_refTexture.width  = rhs.m_refTexture.width;
    m_refTexture.type   = rhs.m_refTexture.type;

    uint32_t bytes    = m_refTexture.type == ImageData::PixelType::pt_rgba ? 4 : 3;
    uint32_t buf_size = m_refTexture.height * m_refTexture.width * bytes;

    auto buffer = std::make_unique<uint8_t[]>(buf_size);
    std::memcpy(buffer.get(), rhs.m_refTexture.data.get(), buf_size);
    m_refTexture.data = std::move(buffer);
}

MouseCursor & MouseCursor::operator=(const MouseCursor & rhs)
{
    m_filename   = rhs.m_filename;
    m_hotSpotX   = rhs.m_hotSpotX;
    m_hotSpotY   = rhs.m_hotSpotY;
    m_shape      = rhs.m_shape;
    m_isStandart = rhs.m_isStandart;

    m_refTexture.height = rhs.m_refTexture.height;
    m_refTexture.width  = rhs.m_refTexture.width;
    m_refTexture.type   = rhs.m_refTexture.type;

    uint32_t bytes    = m_refTexture.type == ImageData::PixelType::pt_rgba ? 4 : 3;
    uint32_t buf_size = m_refTexture.height * m_refTexture.width * bytes;

    auto buffer = std::make_unique<uint8_t[]>(buf_size);
    std::memcpy(buffer.get(), rhs.m_refTexture.data.get(), buf_size);
    m_refTexture.data = std::move(buffer);

    return *this;
}

bool MouseCursor::load()
{
    if(Core::instance().getFileSystem().isExist(m_filename))
    {
        m_isStandart = !ReadTGA(Core::instance().getFileSystem().getFile(m_filename), m_refTexture);
        return !m_isStandart;
    }

    return false;
}
}   // namespace evnt
