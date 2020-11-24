#ifndef APP_H
#define APP_H

#include "../object/objectmanager.h"
#include "appstate.h"
#include "command.h"
#include "window.h"

namespace evnt
{
class Timer;

class App
{
public:
    using AppStatePtr = std::unique_ptr<AppState>;

    App();
    ~App();

    bool init(int32_t argc, char * argv[]);
    void processInput() {}
    void update();
    void draw() {}                               // call render() for  main window
    void swap() { mp_main_window->present(); }   // swap back buffer
    void terminate();

    bool running() const { return m_is_running; }
    void stop() { m_is_running = false; }

    // App states interfaces
    template<class T, class... Args>
    AppState::StateID addAppState(Args &&... args)
    {
        std::lock_guard lk(m_state_mutex);
        m_states.push_back(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
        return static_cast<AppState::StateID>(m_states.size() - 1);
    }

    AppState::StateID getEndStateID() const { return m_end_state; }
    void              setStartState(AppState::StateID start);
    void              setNextState(AppState::StateID next_state);
    AppState::StateID getStateID(std::string const & state_name);   // if not found return 0
    std::string       getStateName(AppState::StateID id) const;

    Window &        getMainWindow() { return *mp_main_window; }
    ObjectManager & getObjectManager() { return m_obj_mgr; }
    Command const & getAppCommandLineParam() const { return m_command_line; }

    // input queue interface

private:
    void doStateTransition();

    std::unique_ptr<Window> mp_main_window;
    bool                    m_is_running{true};

    mutable std::mutex       m_state_mutex;
    std::vector<AppStatePtr> m_states;
    AppState::StateID        m_cur_state;
    AppState::StateID        m_next_state{-1};
    const AppState::StateID  m_end_state;

    // input queue buffers

    std::unique_ptr<Timer> mp_obj_mgr_clean_timer;
    ObjectManager          m_obj_mgr;
    Command                m_command_line;
};
}   // namespace evnt
#endif   // APP_H
