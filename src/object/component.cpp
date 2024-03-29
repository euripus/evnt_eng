#include "component.h"
#include "../core/exception.h"
#include "entity.h"
#include "objectmanager.h"

#include <iostream>

namespace evnt
{
OBJECT_IMPLEMENT(Component, Object)

void Component::setOwnerInternal(Entity * go)
{
    mp_owner = go;
}

void Component::sendMessage(CmpMsgsTable::msg_id messageIdentifier, std::any msg_data)
{
    if(mp_owner)
    {
        ClassIDType cid = static_cast<ClassIDType>(getClassIDVirtual());
        mp_owner->sendMessage(cid, messageIdentifier, std::move(msg_data));
    }
}

void Component::onAddMessage(Component * rec, CmpMsgsTable::msg_id id, std::any msg_data)
{
    auto tt = static_cast<ClassIDType>(rec->getClassIDVirtual());

    switch(id)
    {
        case CmpMsgsTable::msg_id::mDidAddComponent:
            {
                std::cout << "OnAddComponent type:" << tt << " ID:" << rec->getInstanceId()
                          << " this ID:" << getInstanceId() << std::endl;
            }
    }
}

void Component::dump(int32_t indentLevel) const
{
    std::cout << getClassString() << " { " << std::endl;
    std::cout << std::string(4 * (indentLevel + 1), ' ') << "type: " << getClassIDVirtual() << std::endl;
    std::cout << std::string(4 * (indentLevel + 1), ' ') << "ID: " << getInstanceId() << std::endl;

    std::cout << std::string(4 * (indentLevel + 1), ' ') << "mGameObj {" << std::endl;
    if(mp_owner == nullptr)
    {
        std::cout << "nullptr";
    }
    else
    {
        std::cout << std::string(4 * (indentLevel + 2), ' ');
        std::cout << "Owner object ID: " << mp_owner->getInstanceId();
        std::cout << std::endl;
        std::cout << std::string(4 * (indentLevel + 1), ' ');
    }
    std::cout << "}" << std::endl;
    std::cout << std::string(4 * indentLevel, ' ') << "}";
}

void Component::write(OutputMemoryStream & inMemoryStream, ObjectManager const & gmgr) const
{
    inMemoryStream.write(getClassIDVirtual());
    inMemoryStream.write(getInstanceId());

    if(mp_owner == nullptr)
    {
        int32_t zero{0};
        inMemoryStream.write(zero);
    }
    else
    {
        int32_t one{1};
        inMemoryStream.write(one);
        inMemoryStream.write(mp_owner->getInstanceId());
    }
}

void Component::read(InputMemoryStream const & inMemoryStream, ObjectManager & gmgr)
{
    int32_t type_id{0};
    inMemoryStream.read(type_id);
    uint32_t inst_id{0};
    inMemoryStream.read(inst_id);

    setInstanceId(inst_id);

    int32_t n_ptr{0};
    inMemoryStream.read(n_ptr);

    if(n_ptr != 0)
    {
        // read instance id
        inMemoryStream.read(n_ptr);
        // store owner id, actual pointer will be find in linking
        mp_owner = reinterpret_cast<Entity *>(n_ptr);
    }
    else
    {
        mp_owner = nullptr;
    }
}

void Component::link(ObjectManager & gmgr, std::map<uint32_t, uint32_t> const & id_remap)
{
    // https://stackoverflow.com/questions/22419063/error-cast-from-pointer-to-smaller-type-int32_t-loses-information-in-eaglview-mm
    uint32_t inst_id = static_cast<uint32_t>(reinterpret_cast<size_t>(mp_owner));

    if(id_remap.find(inst_id) == id_remap.end())
        EV_EXCEPT("Trying linking not exist object");

    mp_owner = static_cast<Entity *>(gmgr.getObjectPtr(id_remap.at(inst_id)));
}
}   // namespace evnt
