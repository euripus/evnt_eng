#ifndef MOUSECURSOR_H
#define MOUSECURSOR_H

#include "../resource/imagedata.h"
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

    MouseCursor(const MouseCursor & rhs);
    MouseCursor & operator=(const MouseCursor & rhs);

    void setFilename(std::string path)
    {
        m_is_standart = false;
        m_filename    = std::move(path);
    }
    std::string getFilename() const { return m_filename; }

    void setHotspotX(int x) { m_hot_spot_x = x; }
    int  getHotspotX() const { return m_hot_spot_x; }
    void setHotspotY(int y) { m_hot_spot_y = y; }
    int  getHotspotY() const { return m_hot_spot_y; }

    StdCursorShape getStdShape() const { return m_shape; }
    void           setStdShape(StdCursorShape s)
    {
        m_is_standart = true;
        m_shape       = s;
    }
    bool isStdShapeCursor() const { return m_is_standart; }

    const ImageData & getTexture() const { return m_ref_texture; }

    bool load();

    static MouseCursor::StdCursorShape GetShape(std::string const & shape_name);

private:
    std::string m_filename;
    ImageData   m_ref_texture;
    int         m_hot_spot_x;
    int         m_hot_spot_y;

    StdCursorShape m_shape;
    bool           m_is_standart;
};
}   // namespace evnt

#endif   // MOUSECURSOR_H
