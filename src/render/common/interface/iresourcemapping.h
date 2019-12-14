#ifndef IRESOURCEMAPPING_H
#define IRESOURCEMAPPING_H

#include "ideviceobject.h"

namespace evnt
{
/// Describes the resourse mapping object entry
struct ResourceMappingEntry
{
    /// Object name
    std::string name;
    /// Pointer to the object's interface
    IDeviceObject * p_object    = nullptr;
    uint32_t        array_index = 0;

    /// Initializes the structure members
    /// \param [in] _Name       - Object name.
    /// \param [in] _pObject    - Pointer to the object.
    /// \param [in] _ArrayIndex - For array resources, index in the array
    ResourceMappingEntry(const char * _name, IDeviceObject * _p_object, uint32_t _array_index = 0) noexcept :
        name(_name), p_object(_p_object), array_index(_array_index)
    {}
    ResourceMappingEntry() noexcept {}
};

/// Resource mapping description
struct ResourceMappingDesc
{
    /// Pointer to the array of resource mapping entries.
    /// The last element in the array must be default value
    /// created by ResourceMappingEntry::ResourceMappingEntry()
    ResourceMappingEntry * p_entries = nullptr;

    ResourceMappingDesc() noexcept {}
    explicit ResourceMappingDesc(ResourceMappingEntry * _p_entries) noexcept : p_entries(_p_entries) {}
};

/// Resouce mapping
/// This interface provides mapping between literal names and resource pointers.
/// It is created by IRenderDevice::CreateResourceMapping().
/// \remarks Resource mapping holds strong references to all objects it keeps.
class IResourceMapping
{
public:
    /// Adds a resource to the mapping.
    /// \param [in] Name - Resource name.
    /// \param [in] pObject - Pointer to the object.
    /// \param [in] bIsUnique - Flag indicating if a resource with the same name
    ///                         is allowed to be found in the mapping. In the latter
    ///                         case, the new resource replaces the existing one.
    /// \remarks Resource mapping increases the reference counter for referenced objects. So an
    ///          object will not be released as long as it is in the resource mapping.
    virtual void addResource(const char * Name, IDeviceObject * pObject, bool bIsUnique) = 0;
    /// Adds resource array to the mapping.
    /// \param [in] Name - Resource array name.
    /// \param [in] StartIndex - First index in the array, where the first element will be inserted
    /// \param [in] ppObjects - Pointer to the array of objects.
    /// \param [in] NumElements - Number of elements to add
    /// \param [in] bIsUnique - Flag indicating if a resource with the same name
    ///                         is allowed to be found in the mapping. In the latter
    ///                         case, the new resource replaces the existing one.
    /// \remarks Resource mapping increases the reference counter for referenced objects. So an
    ///          object will not be released as long as it is in the resource mapping.
    virtual void addResourceArray(const char * Name, uint32_t StartIndex, IDeviceObject * const * ppObjects,
                                  uint32_t NumElements, bool bIsUnique) = 0;
    /// Removes a resource from the mapping using its literal name.
    /// \param [in] Name - Name of the resource to remove.
    /// \param [in] ArrayIndex - For array resources, index in the array
    virtual void removeResourceByName(const char * Name, uint32_t ArrayIndex = 0) = 0;
    /// Finds a resource in the mapping.
    /// \param [in] Name - Resource name.
    /// \param [in] ArrayIndex - for arrays, index of the array element.
    /// \param [out] ppResource - Address of the memory location where the pointer
    ///                           to the object with the given name will be written.
    ///                           If no object is found, nullptr will be written.
    /// \remarks The method increases the reference counter
    ///          of the returned object, so Release() must be called.
    virtual void getResource(const char * Name, IDeviceObject ** ppResource, uint32_t ArrayIndex = 0) = 0;
    /// Returns the size of the resource mapping, i.e. the number of objects.
    virtual size_t getSize() = 0;
};
}   // namespace evnt

#endif   // IRESOURCEMAPPING_H
