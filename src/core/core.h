#ifndef CORE_H
#define CORE_H

#include <boost/property_tree/ptree.hpp>

#include "../fs/file_system.h"
#include "../app/app.h"
#include "event.h"
#include "gameobject.h"
#include "gameobjectmanager.h"
#include "module.h"

namespace pt = boost::property_tree;

namespace evnt
{
int64_t GetMilisecFromStart();

DECLARE_EVENT_TRAIT(evExit, void);

class Core : public Module<Core>
{
    inline static const uint32_t MS_PER_UPDATE = 16;
    uint32_t                     m_fps{0};

    pt::ptree                    m_root_config;
    std::unique_ptr<ThreadPool>  m_thread_pool;
    std::unique_ptr<EventSystem> m_event_system;
    std::unique_ptr<FileSystem>  m_file_system;
    std::unique_ptr<App>         m_app;

    bool m_running{false};

public:
    Core();

    ~Core() override = default;

    // Events
    template<typename EventTrait>
    void registerEvent()
    {
        m_event_system->registerEvent<EventTrait>();
    }

    template<typename EventTrait>
    EvntHandle addFunctor(typename EventTrait::DelegateType fn)
    {
        return m_event_system->subscribeToEvent<EventTrait>(std::move(fn));
    }

    template<typename EventTrait>
    void removeFunctor(EvntHandle evh)
    {
        m_event_system->unSubscribeFromEvent<EventTrait>(evh);
    }

    template<typename EventTrait, typename... Args>
    auto raiseEvent(Args &&... args)
    {
        static_assert(EventTrait::numArgs == sizeof...(Args), "Incorrect arguments number!");

        return m_event_system->raiseEvent<EventTrait>(std::forward<Args>(args)...);
    }

    template<class T, class... Args>
    void appCreate(Args &&... args)
    {
        m_app = std::make_unique<T>(std::forward<Args>(args)...);
    }
    void appInit(int argc, char * argv[]) { m_app->init(argc, argv); }
    void enterMainLoop();

    void exit() { m_running = false; }

    // getters
    ThreadPool &      getThreadPool() { return *m_thread_pool; }
    FileSystem &      getFileSystem() { return *m_file_system; }
    const pt::ptree & getRootConfig() const { return m_root_config; }
    App &             getApp() { return *m_app; }
};
}   // namespace evnt
#endif   // CORE_H
