#ifndef MOUSECURSOR_H
#define MOUSECURSOR_H

#include "../assets/imagedata.h"
#include <string>

namespace evnt
{
class MouseCursor
{
public:
    enum class StdCursorShape
    {
        ARROW,
        IBEAM,
        CROSSHAIR,
        HAND,
        HRESIZE,
        VRESIZE
    };

    MouseCursor();
    ~MouseCursor();

    MouseCursor(MouseCursor const & rhs);
    MouseCursor & operator=(MouseCursor const & rhs);

    void setFilename(std::string path)
    {
        m_is_standart = false;
        m_filename    = std::move(path);
    }
    std::string getFilename() const { return m_filename; }

    void    setHotspotX(int32_t x) { m_hot_spot_x = x; }
    int32_t getHotspotX() const { return m_hot_spot_x; }
    void    setHotspotY(int32_t y) { m_hot_spot_y = y; }
    int32_t getHotspotY() const { return m_hot_spot_y; }

    StdCursorShape getStdShape() const { return m_shape; }
    void           setStdShape(StdCursorShape s)
    {
        m_is_standart = true;
        m_shape       = s;
    }
    bool isStdShapeCursor() const { return m_is_standart; }

    ImageData const & getTexture() const { return m_ref_texture; }

    bool load();

    static MouseCursor::StdCursorShape GetShape(std::string const & shape_name);

private:
    std::string m_filename;
    ImageData   m_ref_texture;
    int32_t     m_hot_spot_x;
    int32_t     m_hot_spot_y;

    StdCursorShape m_shape;
    bool           m_is_standart;
};
}   // namespace evnt

#endif   // MOUSECURSOR_H
