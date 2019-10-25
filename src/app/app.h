#ifndef APP_H
#define APP_H

#include "../core/objectmanager.h"
#include "appstate.h"
#include "window.h"

namespace evnt
{
class App
{
public:
    using AppStatePtr = std::unique_ptr<AppState>;

    App();
    ~App() = default;

    bool init(int argc, char * argv[]);
    void processInput() {}
    void update();
    void draw() {}   // call render() for  main window
    void terminate() {}

    bool running() const { return m_is_running; }// && m_main_window->getWindowRunning(); }
    void stop() { m_is_running = false; }

    template<class T, class... Args>
    AppState::StateID addAppState(Args &&... args)
    {
        std::lock_guard lk(m_state_mutex);
        m_states.push_back(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
        return m_states.size() - 1;
    }
    AppState::StateID getNullStateID() const { return m_null_state; }
    void              setStartState(AppState::StateID start);
    void              setNextState(AppState::StateID next_state);

    // ?????
    Window &        getMainWindow() { return *m_main_window; }
    ObjectManager & getObjectManager() { return m_obj_mgr; }

private:
    void doStateTransition();

    std::unique_ptr<Window> m_main_window;
    bool                    m_is_running{true};

    mutable std::mutex       m_state_mutex;
    std::vector<AppStatePtr> m_states;
    AppState::StateID        m_cur_state{-1};
    AppState::StateID        m_next_state{-1};
    const AppState::StateID  m_null_state;

    ObjectManager m_obj_mgr;
};
}   // namespace evnt
#endif   // APP_H
