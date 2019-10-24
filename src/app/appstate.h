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
    using StateID = int32_t;

    AppState(App & owner) : m_owner{owner} {}
    virtual ~AppState() = default;

    /// called when state is created
    virtual void init() {}
    /// called when state is becoming active
    virtual void onStateEnter(const StateID prevState) {}
    /// called when state is becoming inactive
    virtual void onStateLeave(const StateID nextState) {}
    /// called on state to perform state logic
    virtual void update() {}
    /// called on state is terminating, switch to next state
    virtual void terminate() = 0;

protected:
    App & m_owner;
};

class null_state : public AppState
{
public:
    null_state(App & owner) : AppState(owner) {}
    ~null_state() override = default;

    void update() override;
    void terminate() override;
};
}   // namespace evnt

#endif   // APPSTATE_H
