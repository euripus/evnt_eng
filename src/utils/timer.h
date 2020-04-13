#ifndef TIMER_H
#define TIMER_H

#include "../core/core.h"
#include "../core/exception.h"
#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

namespace evnt
{
class Timer
{
public:
    Timer() = default;
    ~Timer()
    {
        if(m_loop_running)
            m_loop_running = false;

        if(m_background_thread.joinable())
            m_background_thread.join();
    }

    Timer(const Timer &) = delete;
    Timer & operator=(const Timer &) = delete;

    Timer(Timer &&) = delete;
    Timer & operator=(Timer &&) = delete;

    // callback called asynchronously in threadpool
    template<typename FunctionType,
             typename = std::enable_if_t<!std::is_void_v<std::result_of_t<FunctionType()>>>>
    std::future<typename std::result_of_t<FunctionType()>> onceCall(uint32_t        milliseconds_delay,
                                                                    FunctionType && callback)
    {
        using result_type = typename std::result_of_t<FunctionType()>;

        if(m_is_running)
        {
            EV_EXCEPT("Timer is already running");
        }

        auto timer_call = [this, delay = milliseconds_delay,
                           f = std::forward<FunctionType>(callback)]() -> result_type {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            auto res     = f();
            m_is_running = false;

            return res;
        };

        m_delay      = std::chrono::milliseconds(milliseconds_delay);
        m_is_running = true;
        m_start      = std::chrono::high_resolution_clock::now();

        return Core::instance().getThreadPool().submit(std::move(timer_call));
    }

    void onceCall(uint32_t milliseconds_delay, std::function<void()> callback);
    // callback called asynchronously in separate thread
    void loopCall(uint32_t milliseconds_delay, std::function<void()> callback);

    bool     isRunning() const { return m_is_running; }
    bool     isLooped() const { return m_loop_running; }
    void     loopStop() { m_loop_running = false; }
    uint32_t elapsedMilliseconds() const;

private:
    void submit(std::function<void()> func);

    std::chrono::milliseconds                      m_delay;
    std::chrono::high_resolution_clock::time_point m_start;
    std::atomic_bool                               m_is_running{false};
    std::atomic_bool                               m_loop_running{false};
    std::thread                                    m_background_thread;
};
}   // namespace evnt
#endif   // TIMER_H
