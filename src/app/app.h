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

    App()  = default;
    ~App() = default;

    bool init(int argc, char * argv[]) { return true; }
    void processInput() {}
    void update() {}
    void draw() {}   // call render() for  main window
    void terminate() {}

    bool running() const { return m_is_running && m_main_window->getWindowRunning(); }

    template<class T, class... Args>
    void addAppState(Args &&... args)
    {
        std::lock_guard lk(m_state_mutex);
        m_states.push_back(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
    }

    // ?????
    Window &        getMainWindow() { return *m_main_window; }
    ObjectManager & getObjectManager() { return m_obj_mgr; }

private:
    std::unique_ptr<Window> m_main_window;
    bool                    m_is_running{true};

    mutable std::mutex       m_state_mutex;
    std::vector<AppStatePtr> m_states;
    AppState *               m_cur_state_ptr{nullptr};

    ObjectManager m_obj_mgr;
};
}   // namespace evnt
#endif   // APP_H
