#ifndef KEY_BINDING_H
#define KEY_BINDING_H

#include "devices.h"
#include <functional>
#include <memory>
#include <unordered_map>

namespace evnt
{
class KeyBinding
{
public:
    using RunFunctor    = std::function<void()>;
    using KeyBindingPtr = std::shared_ptr<KeyBinding>;

    KeyBinding() = default;

    static void          KeyPressed(int32_t key_code);
    static void          BindKey(int32_t key_code, KeyBindingPtr action);   // replace old or create new
    static KeyBindingPtr CreateAction(int32_t key_code);

    std::string m_description;
    RunFunctor  m_run_fnct;

    inline static std::unordered_map<int32_t, KeyBindingPtr> s_key_bindings;   // key_code, action
};
}   // namespace evnt

#endif   // KEY_BINDING_H
