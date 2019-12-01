#ifndef GLCONTEXT_H
#define GLCONTEXT_H

namespace evnt
{
class GLContext
{
public:
    // typedef HGLRC NativeGLContextType;

    // GLContext(const struct EngineGLCreateInfo & InitAttribs, struct DeviceCaps & DeviceCaps,
    //          const struct SwapChainDesc * pSCDesc);
    ~GLContext();
    void SwapBuffers();

    // NativeGLContextType GetCurrentNativeGLContext();
};
}   // namespace evnt
#endif   // GLCONTEXT_H
