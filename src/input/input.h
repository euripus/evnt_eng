#ifndef INPUT_H
#define INPUT_H

#include "../core/event.h"
#include "devices.h"
#include <functional>
#include <glm/glm.hpp>
#include <string>

namespace evnt
{
class Input
{
public:
    // Events
    Event<void(int32_t, int32_t)> evMouseCursor;   // (int32_t xpos, int32_t ypos)
    Event<void(int32_t, bool)>    evMouseButton;   // (MouseKeyCode keyCode, bool press)
    Event<void(int32_t, int32_t)> evMouseWheel;    // (int32_t xoffset, int32_t yoffset)
    Event<void(int32_t, bool)>    evKeyboard;      // (KeyCode keyCode, bool press)

protected:
    // Keyboard
    int32_t m_key_id;   // Id of the key pressed in the last update
    bool    m_keys_states[Keyboard::KeyCount];

    // Mouse
    glm::ivec2 m_mouse_position;
    int32_t    m_mouse_wheel;
    bool       m_mouse_buttons_state[Mouse::ButtonCount];

    void onCursorPos(int32_t xpos, int32_t ypos);
    void onMouseButton(int32_t button_code, bool press);
    void onMouseWheel(int32_t xoffset, int32_t yoffset);
    void onKey(int32_t key_code, bool press);

public:
    inline static const std::string KEY_NONE{"---"};

    Input();
    virtual ~Input() = default;

    virtual bool init() { return true; }
    virtual void update() = 0;
    void         reset();

    // Mouse
    glm::ivec2 const & getMousePosition() const { return m_mouse_position; }
    void               setMousePosAbs(const glm::ivec2 & mouse_pos) { m_mouse_position = mouse_pos; }
    bool               getMouseButton(int32_t button_id) const;
    int32_t            getMouseButtonClicked() const;
    int32_t            getMouseWheel() const { return m_mouse_wheel; }

    // Keyboard
    int32_t getKeyPressed() const { return m_key_id; }
    bool    isAnyKeyPressed() const { return m_key_id >= 0; }
    bool    isKeyPressed(int32_t key_id) const;

    static std::string GetKeyName(int32_t key);
    static int32_t     GetKeyId(std::string const & key_name);
};
}   // namespace evnt

#endif   // INPUT_H
