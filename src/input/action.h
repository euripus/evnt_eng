#ifndef ACTION_H
#define ACTION_H

#include "devices.h"
#include <functional>
#include <memory>
#include <unordered_map>

namespace evnt
{
class Action
{
public:
    using RunFunctor = std::function<void()>;
    using ActionPtr  = std::shared_ptr<Action>;

    Action() = default;

    static void      KeyPressed(int32_t key_code);
    static void      BindKey(int32_t key_code, ActionPtr action);   // replace old action or create new
    static ActionPtr CreateAction(int32_t key_code);

    std::string m_name;
    std::string m_title;
    std::string m_description;

    RunFunctor m_run_fnct;

    inline static std::unordered_map<int32_t, ActionPtr> s_key_bindings;   // key_code, action
};
}   // namespace evnt

#endif   // ACTION_H
