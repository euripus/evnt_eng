#ifndef IDEVICEOBJECT_H
#define IDEVICEOBJECT_H

#include "../graphics_types.h"

namespace evnt
{
/// Describes common device object attributes
struct DeviceObjectAttribs
{
    /// Object name
    const std::string name;

    // We have to explicitly define constructors because otherwise Apple's clang fails to compile the
    // following legitimate code:
    //     DeviceObjectAttribs{"Name"}

    DeviceObjectAttribs() noexcept {}
    explicit DeviceObjectAttribs(const char * _name) : name(_name) {}
};

/// Base interface for all objects created by the render device evnt::IRenderDevice
class IDeviceObject
{
public:
    /// Returns the buffer object description
    virtual const DeviceObjectAttribs & getDesc() const = 0;
};
}   // namespace evnt

#endif   // IDEVICEOBJECT_H
