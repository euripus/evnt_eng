#ifndef IRENDERDEVICEGL_H
#define IRENDERDEVICEGL_H

#include "../../common/interface/irenderdevice.h"

namespace evnt
{
class IRenderDeviceGL : public IRenderDevice
{
public:
    /// Creates a texture from OpenGL handle
    /// \param [in] GLHandle      - OpenGL texture handle
    /// \param [in] TexDesc       - Texture description. The engine can automatically
    ///                             set texture width, height, depth, mip levels count, and format.
    ///                             Remaining fields should be set up by the app.
    /// \param [in]  InitialState - Initial texture state. See Diligent::RESOURCE_STATE.
    /// \param [out] ppTexture    - Address of the memory location where the pointer to the
    ///                             texture interface will be stored.
    ///                             The function calls AddRef(), so that the new object will contain
    ///                             one refernce.
    /// \note  Diligent engine texture object does not take ownership of the GL resource,
    ///        and the application must not destroy it while it is in use by the engine.
    virtual void createTextureFromGLHandle(uint32_t GLHandle, const TextureDesc & TexDesc,
                                           RESOURCE_STATE InitialState, ITexture ** ppTexture) = 0;

    /// Creates a buffer from OpenGL handle
    /// \param [in] GLHandle      - OpenGL buffer handle
    /// \param [in] BuffDesc      - Buffer description. The engine can automatically
    ///                             recover buffer size, but the rest of the fields need to
    ///                             be set by the client.
    /// \param [in]  InitialState - Initial buffer state. See Diligent::RESOURCE_STATE.
    /// \param [out] ppBuffer     - Address of the memory location where the pointer to the
    ///                             texture interface will be stored.
    ///                             The function calls AddRef(), so that the new object will contain
    ///                             one refernce.
    /// \note  Diligent engine buffer object does not take ownership of the GL resource,
    ///        and the application must not destroy it while it is in use by the engine.
    virtual void createBufferFromGLHandle(uint32_t GLHandle, const BufferDesc & BuffDesc,
                                          RESOURCE_STATE InitialState, IBuffer ** ppBuffer) = 0;
};
}   // namespace evnt

#endif   // IRENDERDEVICEGL_H
