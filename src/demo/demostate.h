#ifndef DEMOSTATE_H
#define DEMOSTATE_H

#include "../app/appstate.h"

namespace evnt
{
class DemoState : public AppState
{
public:
    DemoState(App & owner) : AppState(owner, "demo_state") {}

    virtual bool init() override { return true; }
    virtual void onStateEnter(const StateID prevState) override {}
    virtual void onStateLeave(const StateID nextState) override {}
    virtual void update() override;
    virtual void terminate() override {}
};
}   // namespace evnt
#endif   // DEMOSTATE_H
