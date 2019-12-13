#ifndef RENDERDEVICEGLIMPL_H
#define RENDERDEVICEGLIMPL_H

#include "../common/renderdevicebase.h"
#include "./interface/irenderdevicegl.h"

namespace evnt
{
class RenderDeviceGLImpl : public RenderDeviceBase<IRenderDeviceGL>
{
public:
    RenderDeviceGLImpl();
};
}   // namespace evnt

#endif   // RENDERDEVICEGLIMPL_H
