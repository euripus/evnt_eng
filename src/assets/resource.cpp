#include "resource.h"
#include <algorithm>

#include "../core/core.h"

namespace evnt
{
std::string Resource::GetStringFromResourceType(Resource::ResourceType type)
{
    switch(type)
    {
        case ResourceType::Undefined:
            return "Undefined";
        case ResourceType::Spatial:
            return "Spatial";
        case ResourceType::Geometry:
            return "Geometry";
        case ResourceType::Animation:
            return "Animation";
        case ResourceType::Material:
            return "Material";
        case ResourceType::Shader:
            return "Shader";
        case ResourceType::Texture:
            return "Texture";
        case ResourceType::ParticleEffect:
            return "ParticleEffect";
        case ResourceType::Pipeline:
            return "Pipeline";
    }

    return "Undefined";
}

Resource::ResourceType Resource::GetResourceTypeFromString(std::string const & type_name)
{
    if(type_name == "Spatial")
        return ResourceType::Spatial;
    else if(type_name == "Geometry")
        return ResourceType::Geometry;
    else if(type_name == "Animation")
        return ResourceType::Animation;
    else if(type_name == "Material")
        return ResourceType::Material;
    else if(type_name == "Shader")
        return ResourceType::Shader;
    else if(type_name == "Texture")
        return ResourceType::Texture;
    else if(type_name == "ParticleEffect")
        return ResourceType::ParticleEffect;
    else if(type_name == "Pipeline")
        return ResourceType::Pipeline;

    return ResourceType::Undefined;
}

Resource::Resource(Resource::ResourceType type, std::string name) :
    m_type{type}, m_name{std::move(name)}, m_state{State::state_path}
{}

ResourceManager::~ResourceManager()
{
    for(auto & [type, reg] : m_registry)
    {
        if(reg.res_release_function)
            reg.res_release_function();
    }
}

void ResourceManager::registerType(Resource::ResourceType type, std::string const & type_name,
                                   ResourceManager::ResReadFunc       read_func,
                                   ResourceManager::ResGetDefaultFunc default_func,
                                   ResourceManager::ResWriteFunc      write_func,
                                   ResourceManager::ResInitFunc       init_func,
                                   ResourceManager::ResReleaseFunc    release_func)
{
    ResourceRegEntry reg;
    reg.res_type_name            = type_name;
    reg.res_read_function        = read_func;
    reg.res_get_default_function = default_func;
    reg.res_write_function       = write_func;
    reg.res_init_function        = init_func;
    reg.res_release_function     = release_func;

    registerType(type, std::move(reg));
}

void ResourceManager::registerType(Resource::ResourceType type, ResourceManager::ResourceRegEntry reg_entry)
{
    m_registry[type] = std::move(reg_entry);
    if(m_registry[type].res_init_function)
        m_registry[type].res_init_function();
}

std::string ResourceManager::getTypeName(Resource::ResourceType type) const
{
    auto it = m_registry.find(type);
    if(it != m_registry.end())
        return it->second.res_type_name;

    std::stringstream ss;
    ss << "Error. Query unregistered type: \"" << static_cast<uint32_t>(type) << "\"" << std::endl;
    Log::Log(Log::error, ss.str());
    return {};
}

Resource::ResourceType ResourceManager::getTypeEnum(std::string const & type_name) const
{
    for(auto const & [key, reg] : m_registry)
    {
        if(reg.res_type_name == type_name)
            return key;
    }

    std::stringstream ss;
    ss << "Error. Query unregistered type name: \"" << type_name << "\"" << std::endl;
    Log::Log(Log::error, ss.str());
    return Resource::ResourceType::Undefined;
}

ResourceManager::ResourceRegEntry ResourceManager::getTypeRegEntry(Resource::ResourceType type) const
{
    return m_registry.at(type);
}

Resource::ResourceSharedPtr ResourceManager::getResource(Resource::ResourceType type,
                                                         std::string const &    name)
{
    // if existed
    auto res_shared = getExisted(type, name);
    if(res_shared)
        return res_shared;

    // load resource
    auto reg = getTypeRegEntry(type);
    if(isFileExisted(name))
    {
        res_shared = reg.res_read_function(name);

        std::lock_guard lock(m_resources_guard);
        m_resources[type].push_back(res_shared);
        return res_shared;
    }

    // if file not existed log and return default
    std::stringstream ss;
    ss << "Warning. Resource: \"" << name << "\" "
       << "not existed!" << std::endl;
    Log::Log(Log::warning, ss.str());
    return reg.res_get_default_function();
}

void ResourceManager::releaseUnused()
{
    std::lock_guard lock(m_resources_guard);
    for(auto & [type, list] : m_resources)
    {
        for(auto it = list.begin(); it != list.end();)
        {
            if((*it).expired())
            {
                it = list.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

Resource::ResourceSharedPtr ResourceManager::getExisted(Resource::ResourceType type, const std::string & name)
{
    std::lock_guard lock(m_resources_guard);
    auto &          res_list = m_resources[type];

    // if resource already exists
    for(auto & res : res_list)
    {
        if(!res.expired())
        {
            auto res_shrd = res.lock();
            if(res_shrd->m_name == name)
                return res_shrd;
        }
    }

    return {};
}

bool ResourceManager::isFileExisted(std::string const & name) const
{
    auto fs = Core::instance().getFileSystem();
    return fs.isExist(name);
}

}   // namespace evnt
