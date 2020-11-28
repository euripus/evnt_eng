#ifndef RESOURCE_H
#define RESOURCE_H

#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

namespace evnt
{
class Resource
{
public:
    using ResourcePtr       = std::unique_ptr<Resource>;
    using ResourceSharedPtr = std::shared_ptr<Resource>;
    using ResourceWeakPtr   = ResourceSharedPtr::weak_type;

    enum class ResourceType
    {
        Undefined = 0,
        Spatial,
        Geometry,
        Animation,
        Material,
        Shader,
        Texture,
        ParticleEffect,
        Pipeline
    };

    enum class State
    {
        state_path,     // name only
        state_mem,      // loaded
        state_invalid   // not found
    };

    Resource(ResourceType type, std::string name);
    virtual ~Resource() = default;

    ResourceType getType() const { return m_type; }
    std::string  getName() const { return m_name; }
    State        getState() const { return m_state; }

    virtual bool load() { return true; }
    virtual void release() {}

    static std::string  GetStringFromResourceType(ResourceType type);
    static ResourceType GetResourceTypeFromString(std::string const & type_name);

    static ResourceType GetTypeID() { return ResourceType::Undefined; }

protected:
    ResourceType m_type = {ResourceType::Undefined};
    std::string  m_name;
    State        m_state;

    friend class ResourceManager;
};

class ResourceManager
{
public:
    using ResReadFunc       = std::function<Resource::ResourceSharedPtr(std::string const &)>;
    using ResGetDefaultFunc = std::function<Resource::ResourceSharedPtr()>;
    using ResWriteFunc      = std::function<bool(Resource const &)>;
    using ResInitFunc       = std::function<void()>;
    using ResReleaseFunc    = std::function<void()>;

    struct ResourceRegEntry
    {
        std::string       res_type_name;
        ResReadFunc       res_read_function;
        ResGetDefaultFunc res_get_default_function;
        ResWriteFunc      res_write_function;
        ResInitFunc       res_init_function;
        ResReleaseFunc    res_release_function;
    };

    ResourceManager() = default;
    ~ResourceManager();

    void registerType(Resource::ResourceType type, std::string const & type_name, ResReadFunc read_func,
                      ResGetDefaultFunc default_func, ResWriteFunc write_func = {},
                      ResInitFunc init_func = {}, ResReleaseFunc release_func = {});
    void registerType(Resource::ResourceType type, ResourceRegEntry reg_entry);
    std::string            getTypeName(Resource::ResourceType type) const;
    Resource::ResourceType getTypeEnum(std::string const & type_name) const;
    ResourceRegEntry       getTypeRegEntry(Resource::ResourceType type) const;

    Resource::ResourceSharedPtr getResource(Resource::ResourceType type, std::string const & name);
    void                        releaseUnused();

private:
    std::unordered_map<Resource::ResourceType, std::list<Resource::ResourceWeakPtr>> m_resources;
    std::unordered_map<Resource::ResourceType, ResourceRegEntry>                     m_registry;

    std::mutex m_resources_guard;
};
}   // namespace evnt
#endif   // RESOURCE_H
