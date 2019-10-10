#ifndef WINDOW_H
#define WINDOW_H

#include "../input/input.h"
#include "mousecursor.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace evnt
{
struct DisplayMode
{
    glm::ivec2 resolution;

    DisplayMode() {}
    DisplayMode(const DisplayMode & o) : resolution(o.resolution) {}
    /* implicit */ DisplayMode(glm::ivec2 res) : resolution(res) {}

    bool operator<(const DisplayMode & other) const;
    bool operator==(const DisplayMode & other) const { return resolution == other.resolution; }
};

using DisplayModes = std::vector<DisplayMode>;

class Window
{
public:
    enum class AlertType
    {
        AlertType_Information,
        AlertType_Warning,
        AlertType_Error
    };

    Window();
    virtual ~Window();

    virtual void alert(const std::string & title, const std::string & message,
                       AlertType type = AlertType::AlertType_Information) = 0;

    void setWindowTitle(const std::string & title) { m_title = title; }

    Input & getInput() { return *m_input; }

    glm::ivec2        getWindowSize() const { return m_win_size; }
    const DisplayMode getDisplayMode() const { return DisplayMode(m_win_size); }
    bool              getWindowRunning() const { return m_running; }
    bool              isFullScreen() const { return m_full_screen; }
    void              setMSAALevel(int level) { m_MSAA_level = level; }
    int               getMSAALevel() const { return m_MSAA_level; }
    float             getGamma() const { return m_gamma; }
    void              setGamma(float g) { m_gamma = g; }

    // virtual's
    virtual bool create()  = 0;
    virtual void destroy() = 0;
    virtual void pollEvents() const {}

    virtual void         fullscreen(bool fullscreen) = 0;
    virtual DisplayModes getDisplayModes() const     = 0;

    virtual void adjustGamma() {}

    virtual void                setMouseCursor(MouseCursor cursor) = 0;
    virtual const MouseCursor & getMouseCursor() const { return m_cur_mouse_cursor; }
    virtual void                setCursorVisibility(bool type) = 0;
    virtual bool                isCursorVisible() const { return m_cursor_visibility; }

    // factory
    static std::unique_ptr<Window> CreateMainWindow();

    // Events functors
    void winResize(glm::ivec2 sz);
    void winFullscreen();

protected:
    std::string m_title;
    glm::ivec2  m_win_size;
    bool        m_full_screen;
    bool        m_running;
    int         m_MSAA_level;
    float       m_gamma;

    bool        m_cursor_visibility;
    MouseCursor m_cur_mouse_cursor;

    std::unique_ptr<Input> m_input;
};
}   // namespace evnt

#endif   // WINDOW_H
