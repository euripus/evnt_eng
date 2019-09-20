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

    void        setFilename(std::string path) { m_filename = path; }
    std::string getFilename() const { return m_filename; }

    void setHotspotX(int x) { m_hotSpotX = x; }
    int  getHotspotX() const { return m_hotSpotX; }
    void setHotspotY(int y) { m_hotSpotY = y; }
    int  getHotspotY() const { return m_hotSpotY; }

    StdCursorShape getStdShape() const { return m_shape; }
    void           setStdShape(StdCursorShape s) { m_shape = s; }
    bool           isStdShapeCursor() const { return m_isStandart; }

    const ImageData & getTexture() const { return m_refTexture; }

    bool load();

private:
    std::string m_filename;
    ImageData   m_refTexture;
    int         m_hotSpotX;
    int         m_hotSpotY;

    StdCursorShape m_shape;
    bool           m_isStandart;
};
}   // namespace evnt

#endif   // MOUSECURSOR_H
