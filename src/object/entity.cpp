#include "entity.h"
#include "../core/exception.h"
#include "objectmanager.h"

#include <any>
#include <iostream>

namespace evnt
{
OBJECT_IMPLEMENT(Entity, Object)

void Entity::addComponent(PObjHandle com)
{
    assert(com);

    auto cmp_ptr = dynamic_ohdl_cast<evnt::Component>(com);

    cmp_ptr->setOwnerInternal(this);
    cmp_ptr->sendMessage(CmpMsgsTable::mDidAddComponent, {});
    m_components[cmp_ptr->getClassIDVirtual()] = com;
}

Component * Entity::queryComponentImplementation(int32_t classID) const
{
    assert(classID != -1);

    Component * res{nullptr};

    auto id = m_components.find(classID);
    if(id != m_components.end())
    {
        res = dynamic_ohdl_cast<evnt::Component>(id->second);
    }

    return res;
}

void Entity::sendMessage(ClassIDType sender, CmpMsgsTable::msg_id message_identifier, std::any msg_data)
{
    assert(message_identifier != CmpMsgsTable::mUndefined);

    for(auto & [key, cmp] : m_components)
    {
        auto cid = static_cast<ClassIDType>(key);
        if(s_msg_handlers.hasMessageCallback(message_identifier, cid) && cid != sender)
        {
            auto cmp_ptr = dynamic_ohdl_cast<evnt::Component>(cmp);
            s_msg_handlers.handleMessage(cmp_ptr, message_identifier, msg_data);
        }
    }
}

void Entity::dump(int32_t indentLevel) const
{
    std::cout << getClassString() << " { " << std::endl;
    std::cout << std::string(4 * (indentLevel + 1), ' ') << "type: " << getClassIDVirtual() << std::endl;
    std::cout << std::string(4 * (indentLevel + 1), ' ') << "ID: " << getInstanceId() << std::endl;

    std::cout << std::string(4 * (indentLevel + 1), ' ') << std::string("std::unique_ptr<mComponents>")
              << "{";
    if(m_components.empty())
    {
        std::cout << "0}";
        std::cout << std::endl;
    }
    else
    {
        std::cout << std::endl;
        for(auto & [key, cmp] : m_components)
        {
            auto c_ptr = cmp->getPtr();
            std::cout << std::string(4 * (indentLevel + 2), ' ') << "[type: ";
            std::cout << key;
            std::cout << ", ID: " << c_ptr->getInstanceId();
            std::cout << "]" << std::endl;
        }
        std::cout << std::string(4 * (indentLevel + 1), ' ') << "}";
        std::cout << std::endl;
    }
    std::cout << std::string(4 * indentLevel, ' ') << "}";
}

void Entity::write(OutputMemoryStream & inMemoryStream, ObjectManager const & gmgr) const
{
    inMemoryStream.write(getClassIDVirtual());
    inMemoryStream.write(getInstanceId());

    if(m_components.empty())
    {
        uint32_t zero{0};
        inMemoryStream.write(zero);
    }
    else
    {
        uint32_t size = static_cast<uint32_t>(m_components.size());
        inMemoryStream.write(size);
        for(auto & [key, cmp] : m_components)
        {
            auto c_ptr = cmp->getPtr();
            inMemoryStream.write(key);
            inMemoryStream.write(c_ptr->getInstanceId());
        }
    }
}

void Entity::read(InputMemoryStream const & inMemoryStream, ObjectManager & gmgr)
{
    int32_t type_id{0};
    inMemoryStream.read(type_id);
    uint32_t inst_id{0};
    inMemoryStream.read(inst_id);

    setInstanceId(inst_id);

    uint32_t size{0};
    inMemoryStream.read(size);
    if(size > 0)
    {
        while(size > 0)
        {
            int32_t  key{0};
            uint32_t cmp_inst{0};

            inMemoryStream.read(key);
            inMemoryStream.read(cmp_inst);

            auto temp_handle        = std::make_shared<ObjHandle>(nullptr);
            temp_handle->m_link_key = cmp_inst;
            m_components[key]       = temp_handle;

            --size;
        }
    }
}

void Entity::link(ObjectManager & gmgr, std::map<uint32_t, uint32_t> const & id_remap)
{
    if(m_components.empty())
        return;

    for(auto & [key, cmp] : m_components)
    {
        uint32_t c_inst = cmp->m_link_key;

        if(id_remap.find(c_inst) == id_remap.end())
            EV_EXCEPT("Trying linking not exist object");

        auto ptr = gmgr.getObject(id_remap.at(c_inst));

        m_components[key] = ptr;
    }
}
}   // namespace evnt
