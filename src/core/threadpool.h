#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <atomic>
#include <boost/asio.hpp>
#include <future>
#include <thread>
#include <vector>

namespace evnt
{
class ThreadPool
{
private:
    boost::asio::io_service       m_io_serv;
    boost::asio::io_service::work m_work;
    std::vector<std::thread>      m_threads;
    std::atomic_size_t            m_num_tasks;

public:
    ThreadPool(ThreadPool const &)             = delete;
    ThreadPool & operator=(ThreadPool const &) = delete;

    ThreadPool() : m_io_serv(), m_work(m_io_serv), m_num_tasks(0)
    {
        uint32_t    num_cores = std::thread::hardware_concurrency();
        std::size_t pool_size = std::max<uint32_t>(1, num_cores - 1);   // 2 threads on single core system
        createPoolThreads(pool_size);
    }

    ThreadPool(std::size_t pool_size) : m_io_serv(), m_work(m_io_serv), m_num_tasks(0)
    {
        createPoolThreads(pool_size);
    }

    ~ThreadPool()
    {
        // Force all threads to return from io_service::run().
        m_io_serv.stop();

        for(auto & th : m_threads)
        {
            if(th.joinable())
                th.join();
        }
    }

    std::size_t getNumTasks() const { return m_num_tasks; }

    template<typename FunctionType,
             typename = std::enable_if_t<!std::is_void_v<std::result_of_t<FunctionType()>>>>
    auto submit(FunctionType && f)
    {
        using result_type = typename std::result_of_t<FunctionType()>;
        using task_type   = typename std::packaged_task<result_type()>;

        // https://stackoverflow.com/questions/13157502/how-do-you-post-a-boost-packaged-task-to-an-io-service-in-c03
        auto ff = [this, f = std::forward<FunctionType>(f)]() -> result_type {
            return wrapTask(std::move(f));
        };
        auto                     task = std::make_shared<task_type>(std::move(ff));
        std::future<result_type> res  = task->get_future();

        ++m_num_tasks;
        m_io_serv.post(std::bind(&task_type::operator(), task));

        return res;
    }

    void submit(std::function<void()> f)
    {
        auto ff = [this, f = std::move(f)]() {
            wrapTask(std::move(f));
        };

        ++m_num_tasks;
        m_io_serv.post(ff);
    }

private:
    /// Wrap a task so that the available count can be decreased
    template<typename FunctionType,
             typename = std::enable_if_t<!std::is_void_v<std::result_of_t<FunctionType()>>>>
    auto wrapTask(FunctionType f)
    {
        using result_type = typename std::result_of_t<FunctionType()>;

        result_type res = f();
        --m_num_tasks;
        return res;
    }

    void wrapTask(std::function<void()> f)
    {
        // Run the user supplied task.
        try
        {
            f();
        }
        // Suppress all exceptions.
        catch(...)
        {}

        --m_num_tasks;
    }

    void createPoolThreads(std::size_t pool_size)
    {
        for(std::size_t i = 0; i < pool_size; ++i)
        {
            m_threads.emplace_back([this]() { m_io_serv.run(); });
        }
    }
};
}   // namespace evnt

#endif   // THREADPOOL_H
