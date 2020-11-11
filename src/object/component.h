#ifndef COMPONENT_H
#define COMPONENT_H

#include "cmpmsgs.h"
#include "object.h"

namespace evnt
{
class Entity;

class Component : public Object
{
public:
    OBJECT_DEFINE(Component, Object)

    Component() = default;

    void setOwnerInternal(Entity * go);
    void sendMessage(CmpMsgsTable::msg_id messageIdentifier, std::any msg_data);

    // test
    void onAddMessage(Component * rec, CmpMsgsTable::msg_id id, std::any msg_data);

private:
    Entity * mp_owner{nullptr};
};
}   // namespace evnt

#endif   // COMPONENT_H
