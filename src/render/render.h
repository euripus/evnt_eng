#ifndef RENDER_H
#define RENDER_H

#include "../app/window.h"
#include "renderdevice.h"
#include <memory>
#include <vector>

namespace evnt
{
class Render
{
public:
    Render(Window & owner) : m_owner(owner) {}

    void update(double CurrTime, double ElapsedTime);
    void resize(int width, int height);
    void present();

private:
    Window &                      m_owner;
    std::unique_ptr<RenderDevice> mp_device;
};
}   // namespace evnt
#endif   // RENDER_H
