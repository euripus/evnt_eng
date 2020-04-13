#include "timer.h"

namespace evnt
{
uint32_t Timer::elapsedMilliseconds() const
{
    uint32_t res{0};
    if(m_is_running)
    {
        auto now = std::chrono::high_resolution_clock::now();
        res      = static_cast<uint32_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start).count());
    }

    return res;
}

void Timer::onceCall(uint32_t milliseconds_delay, std::function<void()> callback)
{
    if(m_is_running)
    {
        EV_EXCEPT("Timer is already running");
    }

    auto timer_call = [this, delay = milliseconds_delay, f = std::move(callback)]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        f();
        m_is_running = false;
    };

    m_delay      = std::chrono::milliseconds(milliseconds_delay);
    m_is_running = true;
    m_start      = std::chrono::high_resolution_clock::now();

    return Core::instance().getThreadPool().submit(timer_call);
}

void Timer::loopCall(uint32_t milliseconds_delay, std::function<void()> callback)
{
    if(m_is_running)
    {
        EV_EXCEPT("Timer is already running");
    }

    auto timer_call = [this, delay = milliseconds_delay, f = std::move(callback)]() {
        while(m_loop_running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            f();
        }

        m_is_running = false;
    };

    m_delay        = std::chrono::milliseconds(milliseconds_delay);
    m_is_running   = true;
    m_loop_running = true;
    m_start        = std::chrono::high_resolution_clock::now();

    submit(timer_call);
}

void Timer::submit(std::function<void()> func)
{
    m_background_thread = std::thread(std::move(func));
}
}   // namespace evnt
