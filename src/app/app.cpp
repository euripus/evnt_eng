#include "app.h"
#include "../log/log.h"

namespace evnt
{
bool App::init(int argc, char * argv[])
{
    if(m_cur_state == -1)
    {
        Log::Log(Log::error, Log::cstr_log("Not set initial state"));
        return false;
    }

    // command string parse

    // window creeate

    return true;
}

void App::update()
{
    // perform state transition
    if((m_next_state != -1 && (m_next_state != m_cur_state)))
    {
        doStateTransition();
    }

    m_states[m_cur_state]->update();
}

void App::setStartState(AppState::StateID start)
{
    if(start > static_cast<int32_t>(m_states.size() - 1))
        return;

    std::lock_guard lk(m_state_mutex);
    m_cur_state = start;
}

void App::setNextState(AppState::StateID next_state)
{
    assert(next_state > 0 && next_state < m_states.size() - 1);

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
