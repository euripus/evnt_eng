#ifndef APPSTATE_H
#define APPSTATE_H

#include <stdint.h>
#include <string>

namespace evnt
{
// stae interface
class App;
class AppState
{
public:
    using StateID = int32_t;

    AppState(App & owner, std::string state_name) : m_owner{owner}, m_state_name{std::move(state_name)} {}
    virtual ~AppState() = default;

    const std::string & getStateName() const { return m_state_name; }

    /// called when state is created
    virtual void init() {}
    /// called when state is becoming active
    virtual void onStateEnter(const StateID prevState) {}
    /// called when state is becoming inactive
    virtual void onStateLeave(const StateID nextState) {}
    /// called on state to perform state logic
    virtual void update() {}

protected:
    App &       m_owner;
    std::string m_state_name;
};

class end_state : public AppState
{
public:
    end_state(App & owner) : AppState(owner, "end_state") {}
    ~end_state() override = default;

    void update() override;
};
}   // namespace evnt

#endif   // APPSTATE_H
