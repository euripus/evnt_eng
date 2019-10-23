#include "app.h"
#include "../log/log.h"

namespace evnt
{
bool App::init(int argc, char * argv[])
{
    if(m_cur_state_ptr == nullptr)
    {
        Log::Log(Log::error, Log::cstr_log("Not set initial state"));
        return false;
    }

    // command string parse

    // window creeate

    return true;
}

void App::setStartState(StateID start)
{
    if(start > m_states.size() - 1)
        return;

    std::lock_guard lk(m_state_mutex);
    m_cur_state_ptr = m_states[start].get();
}

void App::doStateTransition()
{
    assert(m_next_state != nullptr);

    if(m_cur_state_ptr != nullptr)
    {
        assert(m_cur_state_ptr != m_next_state);
        m_cur_state_ptr->OnStateLeave(*m_next_state);
        m_next_state->OnStateEnter(*m_cur_state_ptr);
    }
    else
    {
        // m_next_state->OnStateEnter(null_state{});
    }

    std::lock_guard lk(m_state_mutex);
    m_cur_state_ptr = m_next_state;
}
}   // namespace evnt
