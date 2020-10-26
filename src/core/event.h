#ifndef EVENT_H
#define EVENT_H

#include "threadpool.h"

#include <functional>
#include <list>
#include <tuple>

namespace evnt
{
struct FlagLock
{
    explicit FlagLock(std::atomic_bool & f) : m_flag(f)
    {
        while(m_flag.exchange(true))
            ;
    }

    ~FlagLock() { m_flag.store(false); }

private:
    std::atomic_bool & m_flag;
};

using DelegateHandle = std::size_t;

template<typename EventTrait>
class Event;

template<typename RetType, typename... Args>
class Event<RetType(Args...)>
{
public:
    using ResultType   = RetType;
    using ParamsTuple  = std::tuple<Args...>;
    using DelegateType = std::function<RetType(Args...)>;

    static constexpr std::size_t num_args = std::tuple_size<ParamsTuple>::value;

    Event();
    explicit Event(ThreadPool & pool) : m_access_flag(false), m_thread_pool{pool} {}

    DelegateHandle bind(DelegateType fn)
    {
        FlagLock       lk(m_access_flag);
        DelegateHandle evh = ++m_next_available_ID;
        m_delegates.push_back(DelegateHolder{fn, evh});

        return evh;
    }

    void unbind(DelegateHandle dgh)
    {
        FlagLock lk(m_access_flag);
        m_delegates.erase(std::remove_if(m_delegates.begin(), m_delegates.end(),
                                         [dgh](const DelegateHolder & d) { return d.object == dgh; }),
                          m_delegates.end());
    }

    // https://stackoverflow.com/questions/29638627/perfect-forwarding-for-functions-inside-of-a-templated-c-class
    template<typename... Args2>
    auto call(Args2 &&... args2) const
    {
        std::vector<std::future<ResultType>> res;
        {
            FlagLock lk(m_access_flag);
            for(auto & d : m_delegates)
            {
                std::function<ResultType()> fn = std::bind(d.delegate, std::forward<Args2>(args2)...);
                res.push_back(m_thread_pool.submit(fn));
            }
        }
        return res;
    }

    template<typename... Args2>
    void submit(Args2 &&... args2) const
    {
        FlagLock lk(m_access_flag);
        for(auto & d : m_delegates)
        {
            std::function<void()> fn = std::bind(d.delegate, std::forward<Args2>(args2)...);
            m_thread_pool.submit(fn);
        }
    }

    template<typename... Args2>
    void operator()(Args2 &&... args2) const
    {
        submit(std::forward<Args2>(args2)...);
    }

private:
    struct DelegateHolder
    {
        DelegateType delegate;
        std::size_t  object;
    };

    mutable std::atomic_bool  m_access_flag;
    ThreadPool &              m_thread_pool;
    std::list<DelegateHolder> m_delegates;
    uint32_t                  m_next_available_ID = {0};
};
}   // namespace evnt

#endif   // EVENT_H
