#ifndef WINDOW_H
#define WINDOW_H

#include "../core/event.h"
#include "../input/input.h"
//#include "../render/render.h"
#include "mousecursor.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace evnt
{
class Render;

struct DisplayMode
{
    glm::ivec2 resolution;

    DisplayMode() {}
    DisplayMode(const DisplayMode & o) : resolution(o.resolution) {}
    DisplayMode(glm::ivec2 res) : resolution(std::move(res)) {}

    bool operator<(const DisplayMode & other) const;
    bool operator==(const DisplayMode & other) const { return resolution == other.resolution; }
};

using DisplayModes = std::vector<DisplayMode>;

class App;

class Window
{
public:
    enum class AlertType
    {
        AlertType_Information,
        AlertType_Warning,
        AlertType_Error
    };

    Event<void(int32_t, int32_t)> evResize;

    Window(App & app);
    virtual ~Window();

    virtual void alert(std::string const & title, std::string const & message,
                       AlertType type = AlertType::AlertType_Information) = 0;

    void setWindowTitle(std::string const & title) { m_title = title; }

    Input & getInput() { return *mp_input_backend; }

    glm::ivec2        getWindowSize() const { return m_win_size; }
    const DisplayMode getDisplayMode() const { return DisplayMode(m_win_size); }
    bool              getWindowRunning() const { return m_running; }
    bool              isFullScreen() const { return m_full_screen; }
    void              setMSAALevel(int32_t level) { m_MSAA_level = level; }
    int32_t           getMSAALevel() const { return m_MSAA_level; }
    float             getGamma() const { return m_gamma; }
    void              setGamma(float g) { m_gamma = g; }

    // NVI idiom
    void update();

    // virtual's
    virtual bool init() { return true; }
    virtual void terminate() {}
    virtual void present(uint32_t sync_interval = 1) {}

    virtual void         fullscreen(bool is_fullscreen) = 0;
    virtual DisplayModes getDisplayModes() const        = 0;

    virtual void adjustGamma() {}

    virtual void                setMouseCursor(MouseCursor const & cursor) = 0;
    virtual const MouseCursor & getMouseCursor() const { return m_cur_mouse_cursor; }
    virtual void                setCursorVisibility(bool type) = 0;
    virtual bool                isCursorVisible() const { return m_cursor_visibility; }

    // events
    void winResize(int32_t w, int32_t h);

    // factory
    static std::unique_ptr<Window> CreateMainWindow(std::string const & platform_type, App & owner);

private:
    // NVI for update()
    virtual void platformUpdate() {}

protected:
    App & m_owner;

    std::string m_title;
    glm::ivec2  m_win_size;
    glm::ivec2  m_init_win_size;
    bool        m_full_screen;
    bool        m_running;
    int32_t     m_MSAA_level;
    float       m_gamma;

    bool        m_cursor_visibility;
    MouseCursor m_cur_mouse_cursor;

    // subsystems
    std::unique_ptr<Input>  mp_input_backend;
    std::unique_ptr<Render> mp_renderer;
};
}   // namespace evnt

#endif   // WINDOW_H
