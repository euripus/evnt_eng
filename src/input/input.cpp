#include "input.h"
#include <algorithm>
#include <cassert>

namespace evnt
{
Input::Input() : m_key_id{-1}, m_mouse_mode{Mouse::Absolute}, m_mouse_position{0, 0}, m_mouse_wheel{0}
{
    reset();
}

void Input::reset()
{
    std::fill(std::begin(m_keys_states), std::end(m_keys_states), false);
    std::fill(std::begin(m_mouse_buttons_state), std::end(m_mouse_buttons_state), false);
    m_key_id      = -1;
    m_mouse_wheel = 0;
}

bool Input::getMouseButton(int32_t buttonId) const
{
    assert(buttonId >= Mouse::ButtonBase && buttonId < Mouse::ButtonMax);

    int32_t buttonIdx = buttonId - Mouse::ButtonBase;
    return m_mouse_buttons_state[buttonIdx];
}

int Input::getMouseButtonClicked() const
{
    int32_t buttonIdx = -1;

    for(int32_t i = Mouse::ButtonBase; i < Mouse::ButtonMax; i++)
    {
        buttonIdx = i - Mouse::ButtonBase;
        if(m_mouse_buttons_state[i])
        {
            return i;
        }
    }
    return buttonIdx;
}

bool Input::isKeyPressed(int keyId) const {}
}   // namespace evnt
