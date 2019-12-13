#ifndef RENDERDEVICEBASE_H
#define RENDERDEVICEBASE_H

#include "objectbase.h"

namespace evnt
{
/// Base implementation of a render device
/// \tparam BaseInterface - base interface that this class will inheret.
/// \warning
/// Render device must *NOT* hold strong references to any
/// object it creates to avoid circular dependencies.
/// Device context, swap chain and all object the device creates
/// keep strong reference to the device.
/// Device only holds weak reference to the immediate context.
template<typename BaseInterface>
class RenderDeviceBase : public ObjectBase<BaseInterface>
{
public:
};
}   // namespace evnt

#endif   // RENDERDEVICEBASE_H
