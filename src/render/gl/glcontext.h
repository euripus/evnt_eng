#ifndef GLCONTEXT_H
#define GLCONTEXT_H

class GLFWwindow;

namespace evnt
{
class GLContext
{
public:
    using NativeGLContextType = GLFWwindow *;

    GLContext(const struct RenderCreateInfo & InitAttribs, struct DeviceCaps & DeviceCaps,
              const struct SwapChainDesc * pSCDesc);
    ~GLContext();
    void SwapBuffers();

    NativeGLContextType GetCurrentNativeGLContext();
};
}   // namespace evnt

#endif   // GLCONTEXT_H
