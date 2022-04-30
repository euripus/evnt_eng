#ifndef CORE_H
#define CORE_H

#include <boost/property_tree/ptree.hpp>

#include "../app/app.h"
#include "../fs/file_system.h"
#include "event.h"
#include "module.h"

namespace pt = boost::property_tree;

namespace evnt
{
int64_t GetMilisecFromStart();

class Core : public Module<Core>
{
    inline static uint32_t const MS_PER_UPDATE = 16;   // roughly 60 fps
    uint32_t                     m_fps{0};

    pt::ptree                   m_root_config;
    std::unique_ptr<ThreadPool> mp_thread_pool;
    std::unique_ptr<FileSystem> mp_file_system;
    std::unique_ptr<App>        mp_app;

    bool m_running{false};

public:
    // Eents
    Event<void()> evExit;

    Core();

    ~Core() override = default;

    bool appInit(int32_t argc,
                 char *  argv[]);   // call after all AppState instances added and start state set
    void enterMainLoop();

    void exit() { m_running = false; }

    // getters
    ThreadPool &      getThreadPool() { return *mp_thread_pool; }
    FileSystem &      getFileSystem() { return *mp_file_system; }
    pt::ptree const & getRootConfig() const { return m_root_config; }
    App &             getApp() { return *mp_app; }
    uint32_t          getFPS() const { return m_fps; }
};

template<typename RetType, typename... Args>
Event<RetType(Args...)>::Event() : m_access_flag(false), m_thread_pool{Core::instance().getThreadPool()}
{}
}   // namespace evnt
#endif   // CORE_H
