#include "object.h"
#include "exception.h"

#include <cassert>
#include <sstream>
#include <stdexcept>

namespace evnt
{
Object::StaticObjectInit Object::sm_class_register{Object::InitType};

void Object::InitType()
{
    RegisterClass(ClassName(Object), -1, "Object", sizeof(Object), Object::CreateInstance);
}

Object::RTTI & Object::ClassIDToRTTI(int32_t classID)
{
    assert(classID != -1);

    auto id = s_classid_to_rtti_map.find(classID);

    if(id == s_classid_to_rtti_map.end())
    {
        std::stringstream ss;
        ss << "Error! Rtti information for class_id:" << classID << "  not found";
        EV_EXCEPT(ss.str());
    }

    return id->second;
}

void Object::RegisterClass(int32_t inClassID, int32_t inBaseClass, const std::string & inName, int32_t size,
                           CreateFunc inFunc)
{
    assert(inClassID != -1);
    assert(s_classid_to_rtti_map.find(inClassID) == s_classid_to_rtti_map.end());

    RTTI rtti;

    rtti.base      = inBaseClass;
    rtti.size      = size;
    rtti.className = inName;
    rtti.factory   = std::move(inFunc);

    s_classid_to_rtti_map[inClassID] = rtti;
}

bool Object::IsDerivedFromClassID(int32_t classID, int32_t derivedFromClassID)
{
    int32_t search = classID;

    while(search != -1)
    {
        if(search == derivedFromClassID)
        {
            return true;
        }
        search = GetSuperClassID(search);
    }

    return false;
}

int32_t Object::GetSuperClassID(int32_t classID)
{
    int32_t base = -1;
    auto    id   = s_classid_to_rtti_map.find(classID);

    if(id != s_classid_to_rtti_map.end())
        base = id->second.base;

    return base;
}

int32_t Object::StringToClassID(const std::string & classString)
{
    int32_t result = -1;

    /// TODO perfomance check needed
    for(const auto & [key, rtti] : s_classid_to_rtti_map)
    {
        if(rtti.className == classString)
        {
            result = key;
            break;
        }
    }

    return result;
}

std::string Object::ClassIDToString(int32_t classID)
{
    std::string result;
    auto        id = s_classid_to_rtti_map.find(classID);

    if(id != s_classid_to_rtti_map.end())
        result = id->second.className;

    return result;
}
}   // namespace evnt
