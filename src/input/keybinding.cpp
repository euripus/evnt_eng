#include "keybinding.h"

namespace evnt
{
void KeyBinding::KeyPressed(int32_t key_code)
{
    auto action = s_key_bindings.find(key_code);

    if(action != s_key_bindings.end())
        if(action->second->m_run_fnct)
            action->second->m_run_fnct();
}

void KeyBinding::BindKey(int32_t key_code, KeyBinding::KeyBindingPtr new_action)
{
    auto action = s_key_bindings.find(key_code);

    if(action != s_key_bindings.end())
        action->second = new_action;
    else
    {
        s_key_bindings[key_code] = new_action;
    }
}

KeyBinding::KeyBindingPtr KeyBinding::CreateAction(int32_t key_code)
{
    auto action = s_key_bindings.find(key_code);

    if(action != s_key_bindings.end())
        return action->second;
    else
    {
        auto new_action          = std::make_shared<KeyBinding>();
        s_key_bindings[key_code] = new_action;
        return new_action;
    }
}
}   // namespace evnt
