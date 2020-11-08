#ifndef DEMOSTATE_H
#define DEMOSTATE_H

#include "../app/appstate.h"

namespace evnt
{
class DemoState : public AppState
{
public:
    DemoState(App & owner) : AppState(owner, "demo_state") {}

    // AppState interface
public:
    bool init() override { return true; }
    void onStateEnter(const StateID prevState) override {}
    void onStateLeave(const StateID nextState) override {}
    void update() override;
    void terminate() override {}

private:
    bool m_fullscreen = false;
};
}   // namespace evnt
#endif   // DEMOSTATE_H
