#include "app.h"
#include "../core/core.h"
#include "../log/log.h"

namespace evnt
{
App::App() : m_end_state{addAppState<end_state>(*this)} {}

bool App::init(int argc, char * argv[])
{
    if(m_cur_state == -1)
    {
        Log::Log(Log::error, Log::cstr_log("Not set initial state"));
        return false;
    }

    bool init_result{true};

    // command line string parse
    m_command_line = Command(argc, argv);

    // window create
    auto        config   = Core::instance().getRootConfig();
    std::string platform = config.get<std::string>("App.Window.Platform.Type");
    mup_main_window        = Window::CreateMainWindow(platform, *this);
    init_result          = init_result && mup_main_window->init();

    // AppStates init
    {
        std::lock_guard lk(m_state_mutex);
        for(auto & state : m_states)
        {
            init_result = init_result && state->init();
        }
    }

    return init_result;
}

void App::update()
{
    mup_main_window->update();

    // perform state transition
    if((m_next_state != -1 && (m_next_state != m_cur_state)))
    {
        doStateTransition();
    }

    m_states[m_cur_state]->update();
}

void App::terminate()
{
    // AppStates terminate
    {
        std::lock_guard lk(m_state_mutex);
        for(auto & state : m_states)
        {
            state->terminate();
        }
    }

    // window terminate
    mup_main_window->terminate();
}

AppState::StateID App::getStateID(std::string const & state_name)
{
    AppState::StateID found_id{0};
    {
        std::lock_guard lk(m_state_mutex);
        for(uint32_t i = 0; i < m_states.size(); ++i)
        {
            auto & state = m_states[i];
            if(state->getStateName() == state_name)
            {
                found_id = i;
                break;
            }
        }
    }

    return found_id;
}

void App::setStartState(AppState::StateID start)
{
    if(start > static_cast<AppState::StateID>(m_states.size() - 1))
        return;

    std::lock_guard lk(m_state_mutex);
    m_cur_state = start;
}

void App::setNextState(AppState::StateID next_state)
{
    assert(next_state > 0 && next_state < static_cast<AppState::StateID>(m_states.size() - 1));

    std::lock_guard lk(m_state_mutex);
    if(m_cur_state != -1)
    {
        if(m_cur_state != m_next_state)
        {
            m_next_state = next_state;
        }
    }
    else
    {
        m_next_state = next_state;
    }
}

void App::doStateTransition()
{
    assert(m_next_state != -1);

    if(m_cur_state != -1)
    {
        assert(m_cur_state != m_next_state);

        m_states[m_cur_state]->onStateLeave(m_next_state);
        m_states[m_next_state]->onStateEnter(m_cur_state);
    }
    else
    {
        m_states[m_next_state]->onStateEnter(-1);
    }

    std::lock_guard lk(m_state_mutex);
    m_cur_state = m_next_state;
}
}   // namespace evnt
