#ifndef IDEVICECONTEXTGL_H
#define IDEVICECONTEXTGL_H

#include "../../common/interface/idevicecontext.h"

namespace evnt
{
/// Interface to the device context object implemented in OpenGL
class IDeviceContextGL : public IDeviceContext
{
public:
    /// Attaches to the active GL context in the thread.
    /// If an application uses multiple GL contexts, this method must be called before any
    /// other command to let the engine update active context every time when control flow
    /// is passed over from the main application
    /// \return false if there is no active GL context, and true otherwise
    virtual bool UpdateCurrentGLContext() = 0;
};
}   // namespace evnt

#endif   // IDEVICECONTEXTGL_H
