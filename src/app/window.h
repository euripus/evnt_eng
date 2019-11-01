#ifndef WINDOW_H
#define WINDOW_H

#include "../core/event.h"
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

class App;

DECLARE_EVENT_TRAIT(evResize, void, glm::ivec2);
DECLARE_EVENT_TRAIT(evFullscreen, void, bool);

class Window
{
public:
    enum class AlertType
    {
        AlertType_Information,
        AlertType_Warning,
        AlertType_Error
    };

    Window(App & app);
    virtual ~Window() = default;

    virtual void alert(std::string const & title, std::string const & message,
                       AlertType type = AlertType::AlertType_Information) = 0;

    void setWindowTitle(std::string const & title) { m_title = title; }

    Input & getInput() { return *m_input_backend; }

    glm::ivec2        getWindowSize() const { return m_win_size; }
    const DisplayMode getDisplayMode() const { return DisplayMode(m_win_size); }
    bool              getWindowRunning() const { return m_running; }
    bool              isFullScreen() const { return m_full_screen; }
    void              setMSAALevel(int level) { m_MSAA_level = level; }
    int               getMSAALevel() const { return m_MSAA_level; }
    float             getGamma() const { return m_gamma; }
    void              setGamma(float g) { m_gamma = g; }

    // virtual's
    virtual bool init() { return true; }
    virtual void terminate() {}
    virtual void update() {}

    virtual void         fullscreen(bool is_fullscreen) = 0;
    virtual DisplayModes getDisplayModes() const        = 0;

    virtual void adjustGamma() {}

    virtual void                setMouseCursor(MouseCursor cursor) = 0;
    virtual const MouseCursor & getMouseCursor() const { return m_cur_mouse_cursor; }
    virtual void                setCursorVisibility(bool type) = 0;
    virtual bool                isCursorVisible() const { return m_cursor_visibility; }

    // factory
    static std::unique_ptr<Window> CreateMainWindow(std::string const & platform_type, App & owner);

    // Events
    void winResize(glm::ivec2 sz);
    void winFullscreen(bool is_fullscreen);

protected:
    App & m_owner;

    std::string m_title;
    glm::ivec2  m_win_size;
    bool        m_full_screen;
    bool        m_running;
    int         m_MSAA_level;
    float       m_gamma;

    bool        m_cursor_visibility;
    MouseCursor m_cur_mouse_cursor;

    std::unique_ptr<Input> m_input_backend;
};
}   // namespace evnt

#endif   // WINDOW_H
