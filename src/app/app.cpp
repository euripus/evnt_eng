#include "app.h"
#include "../core/core.h"
#include "../core/exception.h"
#include "../log/log.h"
#include "../utils/timer.h"

namespace evnt
{
App::App() : m_end_state{addAppState<end_state>(*this)}, m_obj_mgr_clean_call{std::make_unique<Timer>()}
{
    m_cur_state = m_end_state;
}

App::~App() {}

bool App::init(int32_t argc, char * argv[])
{
    if(m_cur_state == m_end_state)
    {
        // Log::Log(Log::warning, Log::cstr_log("Initial state not set!"));
    }

    bool init_result{true};

    // command line string parse
    m_command_line = Command(argc, argv);

    // window create
    auto        config   = Core::instance().getRootConfig();
    std::string platform = config.get<std::string>("App.Window.Platform.Type");
    mp_main_window       = Window::CreateMainWindow(platform, *this);
    init_result          = init_result && mp_main_window->init();

    // AppStates init
    {
        std::lock_guard lk(m_state_mutex);
        for(auto & state : m_states)
        {
            init_result = init_result && state->init();
        }
    }

    // once per second delete dead objects
    m_obj_mgr_clean_call->loopCall(1000, [this] { m_obj_mgr.releaseStalledObjects(); });

    return init_result;
}

void App::update()
{
    mp_main_window->update();

    // perform state transition
    if((m_next_state != -1 && (m_next_state != m_cur_state)))
    {
        doStateTransition();
    }

    m_states[m_cur_state]->update();   // scene update
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
    mp_main_window->terminate();
}

AppState::StateID App::getStateID(std::string const & state_name)
{
    AppState::StateID found_id{0};   // exit_state by default
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

std::string App::getStateName(AppState::StateID id) const
{
    assert(id < static_cast<AppState::StateID>(m_states.size()));

    std::lock_guard lk(m_state_mutex);
    return m_states[id]->getStateName();
}

void App::setStartState(AppState::StateID start)
{
    assert(start < static_cast<AppState::StateID>(m_states.size()));

    std::lock_guard lk(m_state_mutex);
    m_cur_state = start;
}

void App::setNextState(AppState::StateID next_state)
{
    assert(next_state >= 0 && next_state < static_cast<AppState::StateID>(m_states.size()));

    std::lock_guard lk(m_state_mutex);
    m_next_state = next_state;
}

void App::doStateTransition()
{
    assert(m_next_state != -1);
    assert(m_cur_state != m_next_state);

    m_states[m_cur_state]->onStateLeave(m_next_state);
    m_states[m_next_state]->onStateEnter(m_cur_state);

    std::lock_guard lk(m_state_mutex);
    m_cur_state = m_next_state;
}
}   // namespace evnt
