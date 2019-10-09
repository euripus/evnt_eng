#ifndef INPUT_H
#define INPUT_H

#include "devices.h"
#include <glm/glm.hpp>
#include <string>

namespace evnt
{
class Input
{
    // Keyboard
    int32_t m_key_id;   // Id of the key pressed in the last update
    bool    m_keys_states[Keyboard::KeyCount];

    // Mouse
    Mouse::Mode m_mouse_mode;
    glm::ivec2  m_mouse_position;
    int32_t     m_mouse_wheel;
    bool        m_mouse_buttons_state[Mouse::ButtonCount];

public:
    Input();

    void update(uint32_t time);
    void reset();

    // Mouse
    void setMouseMode(Mouse::Mode mode);

    const glm::ivec2 & getMousePosition() const { return m_mouse_position; }
    void               setMousePosAbs(const glm::ivec2 & mousePos);
    bool               getMouseButton(int32_t buttonId) const;
    int32_t            getMouseButtonClicked() const;
    int32_t            getMouseWheel() const { return m_mouse_wheel; }

    // Keyboard
    int32_t getKeyPressed() const { return m_key_id; }
    bool    isAnyKeyPressed() const { return m_key_id >= 0; }
    bool    isKeyPressed(int keyId) const;

    static std::string   GetKeyName(Keyboard::Key key);
    static Keyboard::Key GetKeyId(const std::string & keyName);
};
}   // namespace evnt

#endif   // INPUT_H
