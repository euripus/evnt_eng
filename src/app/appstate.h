#ifndef APPSTATE_H
#define APPSTATE_H

#include <stdint.h>

namespace evnt
{
// stae interface
class App;
class AppState
{
public:
    AppState(App & owner) : m_owner{owner} {}
    virtual ~AppState() = default;

    virtual bool init() { return true; }
    virtual bool running() { return false; }
    virtual void update() {}
    // !!! change cur_state pointer to next state & init next state
    virtual void terminate() = 0;

    /// called when state is created
    virtual void OnCreate();
    /// called when state is becoming active
    virtual void OnStateEnter(const AppState & prevState);
    /// called when state is becoming inactive
    virtual void OnStateLeave(const AppState & nextState);
    /// called on state to perform state logic
    virtual void OnFrame();
    /// called before nSceneServer::RenderScene()
    virtual void OnFrameBefore();
    /// called after nSceneServer::RenderScene()
    virtual void OnFrameRendered();

protected:
    App & m_owner;
};

class null_state : public AppState
{};
}   // namespace evnt

#endif   // APPSTATE_H
