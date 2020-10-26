#include "objectmanager.h"
#include "../core/exception.h"
#include <cassert>
#include <iostream>
#include <limits>
#include <map>

namespace evnt
{
ObjectManager::~ObjectManager()
{
    std::lock_guard<std::mutex> lk(m_objects_mutex);

    for(auto & [type_id, components] : m_objects)
        for(auto & [instance_id, obj_entry] : components)
        {
            if(auto sp = obj_entry.handle.lock())
                sp->nullify();
        }
}

PObjHandle ObjectManager::createDefaultObj(int32_t obj_type)
{
    PUniqueObjPtr u_ptr = Object::ClassIDToRTTI(obj_type).factory();

    return registerObj(std::move(u_ptr), obj_type);
}

PObjHandle ObjectManager::registerObj(PUniqueObjPtr ob, int32_t obj_type)
{
    const uint32_t instance_id = m_next_available_id.fetch_add(1, std::memory_order_relaxed);
    assert(instance_id != std::numeric_limits<uint32_t>::max());

    ob->setInstanceId(instance_id);
    PObjHandle sp = std::make_shared<ObjHandle>(ob.get());

    ObjEntry new_entry;
    new_entry.unique = std::move(ob);
    new_entry.handle = sp;

    {
        std::lock_guard<std::mutex> lk(m_objects_mutex);
        m_objects[obj_type][instance_id] = std::move(new_entry);
    }

    return sp;
}

bool ObjectManager::objectExists(uint32_t id) const
{
    std::lock_guard<std::mutex> lk(m_objects_mutex);
    for(auto & [type_id, components] : m_objects)
    {
        if(components.find(id) != components.end())
            return true;
    }
    return false;
}

PObjHandle ObjectManager::getObject(uint32_t id)
{
    std::lock_guard<std::mutex> lk(m_objects_mutex);

    for(auto & [type_id, components] : m_objects)
    {
        const auto iterFind = components.find(id);
        if(iterFind != components.end())
            return iterFind->second.handle.lock();
    }

    EV_EXCEPT("Trying to acquire not created object.");
}

Object * ObjectManager::getObjectPtr(uint32_t id)
{
    std::lock_guard<std::mutex> lk(m_objects_mutex);

    for(auto & [type_id, components] : m_objects)
    {
        const auto iterFind = components.find(id);
        if(iterFind != components.end())
            return iterFind->second.unique.get();
    }

    EV_EXCEPT("Trying to acquire not created object.");
}

ObjectManager::ComponentsList const & ObjectManager::getObjectsByType(int32_t cmp_type)
{
    auto cmp_iter = m_objects.find(cmp_type);

    if(cmp_iter != m_objects.end())
        return cmp_iter->second;
    else
        EV_EXCEPT("Trying to acquire not created components.");
}

void ObjectManager::releaseStalledObjects()
{
    std::lock_guard<std::mutex> lk(m_objects_mutex);

    for(auto & [type_id, components] : m_objects)
        for(auto it = components.begin(); it != components.end();)
        {
            if(it->second.unique->isDeleted())
                it = components.erase(it);
            else
                ++it;
        }
}

void ObjectManager::dump() const
{
    std::lock_guard<std::mutex> lk(m_objects_mutex);

    for(auto & [type_id, components] : m_objects)
        for(auto & [instance_id, obj_entry] : components)
        {
            if(!obj_entry.unique->isDeleted())
            {
                obj_entry.unique->dump();
                std::cout << std::endl;
            }
        }
}

void ObjectManager::serialize(OutputMemoryStream & inMemoryStream) const
{
    std::lock_guard<std::mutex> lk(m_objects_mutex);

    for(auto & [type_id, components] : m_objects)
        for(auto & [instance_id, obj_entry] : components)
        {
            if(!obj_entry.unique->isDeleted())
                obj_entry.unique->write(inMemoryStream, *this);
        }
}

void ObjectManager::deserialize(const InputMemoryStream & inMemoryStream, std::vector<PObjHandle> & objects)
{
    objects.clear();
    std::map<uint32_t, uint32_t> istance_id_remap;   // [old_id, new_id]

    while(inMemoryStream.getRemainingDataSize() > 0)
    {
        auto    stream_cur_ptr = inMemoryStream.getCurPosPtr();
        int32_t type_id        = *reinterpret_cast<int32_t *>(const_cast<int8_t *>(stream_cur_ptr));

        auto obj = Object::ClassIDToRTTI(type_id).factory();
        obj->read(inMemoryStream, *this);
        auto old_id = obj->getInstanceId();

        auto obj_handler         = registerObj(std::move(obj), type_id);
        istance_id_remap[old_id] = obj_handler->getPtr()->getInstanceId();

        objects.push_back(obj_handler);
    }

    for(auto & [type_id, components] : m_objects)
        for(auto & [instance_id, obj_entry] : components)
        {
            obj_entry.unique->link(*this, istance_id_remap);
        }
}

}   // namespace evnt
