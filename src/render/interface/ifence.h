#ifndef IFENCE_H
#define IFENCE_H

#include "ideviceobject.h"

namespace evnt
{
/// Fence description
struct FenceDesc : DeviceObjectAttribs
{};

/// Fence interface
/// Fence the methods to manipulate a fence object
class IFence : public IDeviceObject
{
public:
    /// Returns the fence description used to create the object
    const FenceDesc & getDesc() const override = 0;
    /// Returns the last completed value signaled by the GPU
    virtual uint64_t getCompletedValue() = 0;
    /// Resets the fence to the specified value.
    virtual void reset(uint64_t value) = 0;
};
}   // namespace evnt

#endif   // IFENCE_H
