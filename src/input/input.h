#ifndef INPUT_H
#define INPUT_H

#include "devices.h"
#include <functional>
#include <glm/glm.hpp>
#include <string>

namespace evnt
{
class Input
{
public:
    using MouseCursorCallback = std::function<void(int32_t, int32_t)>;   // (int32_t xpos, int32_t ypos)
    using MouseButtonCallback = std::function<void(int32_t, bool)>;   // (MouseKeyCode keyCode, bool press )
    using MouseWheelCallback  = std::function<void(int32_t, int32_t)>;   // (int32_t xoffset, int32_t yoffset)
    using KeyboardCallback    = std::function<void(int32_t, bool)>;      // (KeyCode keyCode, bool press)

protected:
    // Keyboard
    int32_t          m_key_id;   // Id of the key pressed in the last update
    bool             m_keys_states[Keyboard::KeyCount];
    KeyboardCallback m_key_callback;

    // Mouse
    glm::ivec2          m_mouse_position;
    int32_t             m_mouse_wheel;
    bool                m_mouse_buttons_state[Mouse::ButtonCount];
    MouseCursorCallback m_mouse_cursor_callback;
    MouseButtonCallback m_mouse_button_callback;
    MouseWheelCallback  m_mouse_wheel_callback;

    void onCursorPos(int32_t xpos, int32_t ypos);
    void onMouseButton(int32_t button_code, bool press);
    void onMouseWheel(int32_t xoffset, int32_t yoffset);
    void onKey(int32_t key_code, bool press);

public:
    inline static const std::string KEY_NONE{"---"};

    Input();
    virtual ~Input() = default;

    virtual void update() = 0;
    void         reset();

    // Mouse
    const glm::ivec2 & getMousePosition() const { return m_mouse_position; }
    void               setMousePosAbs(const glm::ivec2 & mouse_pos) { m_mouse_position = mouse_pos; }
    bool               getMouseButton(int32_t button_id) const;
    int32_t            getMouseButtonClicked() const;
    int32_t            getMouseWheel() const { return m_mouse_wheel; }
    void               setMouseCursorCallback(MouseCursorCallback cb) { m_mouse_cursor_callback = cb; }
    void               setMouseButtonCallback(MouseButtonCallback cb) { m_mouse_button_callback = cb; }
    void               setMouseWheelCallback(MouseWheelCallback cb) { m_mouse_wheel_callback = cb; }

    // Keyboard
    int32_t getKeyPressed() const { return m_key_id; }
    bool    isAnyKeyPressed() const { return m_key_id >= 0; }
    bool    isKeyPressed(int32_t key_id) const;
    void    setKeyBoardCallback(KeyboardCallback cb) { m_key_callback = cb; }

    static std::string GetKeyName(int32_t key);
    static int32_t     GetKeyId(const std::string & key_name);
};
}   // namespace evnt

#endif   // INPUT_H
