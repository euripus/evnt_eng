#include "component.h"
#include "exception.h"
#include "gameobject.h"
#include "objectmanager.h"

#include <iostream>

namespace evnt
{
OBJECT_IMPLEMENT(Component, Object)

void Component::setOwnerInternal(GameObject * go)
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
        case CmpMsgsTable::msg_id::mDidAddComponent: {
            std::cout << "OnAddComponent type:" << tt << " ID:" << rec->getInstanceId()
                      << " this ID:" << getInstanceId() << std::endl;
        }
    }
}

void Component::dump(int indentLevel) const
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

void Component::write(OutputMemoryStream & inMemoryStream, const ObjectManager & gmgr) const
{
    inMemoryStream.write(getClassIDVirtual());
    inMemoryStream.write(getInstanceId());

    if(mp_owner == nullptr)
    {
        inMemoryStream.write(0);
    }
    else
    {
        inMemoryStream.write(1);
        inMemoryStream.write(mp_owner->getInstanceId());
    }
}

void Component::read(const InputMemoryStream & inMemoryStream, ObjectManager & gmgr)
{
    int32_t type_id{0};
    inMemoryStream.read(type_id);
    uint32_t inst_id{0};
    inMemoryStream.read(inst_id);

    setInstanceId(inst_id);

    int n_ptr{0};
    inMemoryStream.read(n_ptr);

    if(n_ptr != 0)
    {
        // read instance id
        inMemoryStream.read(n_ptr);
        mp_owner = reinterpret_cast<GameObject *>(n_ptr);
    }
    else
    {
        mp_owner = nullptr;
    }
}

void Component::link(ObjectManager & gmgr, const std::map<uint32_t, uint32_t> & id_remap)
{
    // https://stackoverflow.com/questions/22419063/error-cast-from-pointer-to-smaller-type-int-loses-information-in-eaglview-mm
    uint32_t inst_id = static_cast<uint32_t>(reinterpret_cast<size_t>(mp_owner));

    if(id_remap.find(inst_id) == id_remap.end())
        EV_EXCEPT("Trying linking not exist object");

    mp_owner = static_cast<GameObject *>(gmgr.getObjectPtr(id_remap.at(inst_id)));
}
}   // namespace evnt
