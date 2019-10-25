#include "action.h"

namespace evnt
{
void Action::KeyPressed(int32_t key_code)
{
    auto action = s_key_bindings.find(key_code);

    if(action != s_key_bindings.end())
        if(action->second->m_run_fnct)
            action->second->m_run_fnct();
}

void Action::BindKey(int32_t key_code, Action::ActionPtr new_action)
{
    auto action = s_key_bindings.find(key_code);

    if(action != s_key_bindings.end())
        action->second = new_action;
    else
    {
        s_key_bindings[key_code] = new_action;
    }
}

Action::ActionPtr Action::CreateAction(int32_t key_code)
{
    auto action = s_key_bindings.find(key_code);

    if(action != s_key_bindings.end())
        return action->second;
    else
    {
        auto new_action          = std::make_shared<Action>();
        s_key_bindings[key_code] = new_action;
        return new_action;
    }
}
}   // namespace evnt
