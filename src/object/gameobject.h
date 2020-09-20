#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "cmpmsgs.h"
#include "component.h"
#include "objhandle.h"
#include <cassert>

namespace evnt
{
class GameObject : public Object
{
public:
    OBJECT_DEFINE(GameObject, Object)

    GameObject() = default;

    template<class T>
    T & getComponent() const;
    template<class T>
    PObjHandle getComponentHandle() const;
    void       addComponent(PObjHandle com);

    Component * queryComponentImplementation(int32_t classID) const;

    void sendMessage(ClassIDType sender, CmpMsgsTable::msg_id message_identifier, std::any msg_data);

    inline static CmpMsgsTable s_msg_handlers;

private:
    std::unordered_map<int32_t, PObjHandle> m_components;   // [type_id, obj_handle]
};

template<class T>
T & GameObject::getComponent() const
{
    auto com = queryComponentImplementation(T::GetClassIDStatic());

    assert(com != nullptr);
    return *static_cast<T *>(com);
}

template<class T>
PObjHandle GameObject::getComponentHandle() const
{
    auto id = m_components.find(T::GetClassIDStatic());

    if(id != m_components.end())
        return id->second;
    else
        return {};
}
}   // namespace evnt

#endif   // GAMEOBJECT_H
