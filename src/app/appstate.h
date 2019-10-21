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

protected:
    App & m_owner;
};
}   // namespace evnt

#endif   // APPSTATE_H
