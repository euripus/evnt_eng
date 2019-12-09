#ifndef COMPONENT_H
#define COMPONENT_H

#include "object.h"
#include "cmpmsgs.h"

namespace evnt
{
class GameObject;

class Component : public Object
{
public:
    OBJECT_DEFINE(Component, Object)

    Component() = default;

    void setOwnerInternal(GameObject * go);
    void sendMessage(CmpMsgsTable::msg_id messageIdentifier, std::any msg_data);

    // test
    void onAddMessage(Component * rec, CmpMsgsTable::msg_id id, std::any msg_data);

private:
    GameObject * mp_owner{nullptr};
};
}   // namespace evnt

#endif   // COMPONENT_H
