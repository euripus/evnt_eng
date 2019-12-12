#ifndef DEVICEOBJECTBASE_H
#define DEVICEOBJECTBASE_H

#include "./interface/ideviceobject.h"
#include <sstream>

namespace evnt
{
/// Template class implementing base functionality for a device object
template<typename RenderDeviceImplType, typename ObjectDescType>
class DeviceObjectBase : public IDeviceObject
{
public:
    /// \param pDevice - pointer to the render device
    /// \param ObjDesc - object description
    DeviceObjectBase(RenderDeviceImplType * p_device, const ObjectDescType & obj_desc) :
        mp_owner_device(p_device), m_desc(obj_desc)
    {}

    DeviceObjectBase(const DeviceObjectBase &) = delete;
    DeviceObjectBase(DeviceObjectBase &&)      = delete;
    DeviceObjectBase & operator=(const DeviceObjectBase &) = delete;
    DeviceObjectBase & operator=(DeviceObjectBase &&) = delete;

    virtual ~DeviceObjectBase() {}

    const ObjectDescType & getDesc() const override final { return m_desc; }
    RenderDeviceImplType * getDevice() const { return mp_owner_device; }

protected:
    /// Pointer to the device
    RenderDeviceImplType * const mp_owner_device;
    /// Object description
    ObjectDescType m_desc;
};
}   // namespace evnt

#endif   // DEVICEOBJECTBASE_H
