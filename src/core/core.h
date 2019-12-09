#ifndef CORE_H
#define CORE_H

#include <boost/property_tree/ptree.hpp>

#include "../app/app.h"
#include "../fs/file_system.h"
#include "event.h"
#include "gameobject.h"
#include "module.h"
#include "objectmanager.h"

namespace pt = boost::property_tree;

namespace evnt
{
int64_t GetMilisecFromStart();

DECLARE_EVENT_TRAIT(evExit, void);

class Core : public Module<Core>
{
    inline static const uint32_t MS_PER_UPDATE = 16;   // roughly 60 fps
    uint32_t                     m_fps{0};

    pt::ptree                    m_root_config;
    std::unique_ptr<ThreadPool>  mup_thread_pool;
    std::unique_ptr<EventSystem> mup_event_system;
    std::unique_ptr<FileSystem>  mup_file_system;
    std::unique_ptr<App>         mup_app;

    bool m_running{false};

public:
    Core();

    ~Core() override = default;

    // Events
    template<typename EventTrait>
    void registerEvent()
    {
        mup_event_system->registerEvent<EventTrait>();
    }

    template<typename EventTrait>
    EvntHandle addFunctor(typename EventTrait::DelegateType fn)
    {
        return mup_event_system->subscribeToEvent<EventTrait>(std::move(fn));
    }

    template<typename EventTrait>
    void removeFunctor(EvntHandle evh)
    {
        mup_event_system->unSubscribeFromEvent<EventTrait>(evh);
    }

    template<typename EventTrait, typename... Args>
    auto raiseEvent(Args &&... args)
    {
        static_assert(EventTrait::numArgs == sizeof...(Args), "Incorrect arguments number!");

        return mup_event_system->raiseEvent<EventTrait>(std::forward<Args>(args)...);
    }

    bool appInit(int argc, char * argv[]);   // call after all AppState instances added and start state set
    void enterMainLoop();

    void exit() { m_running = false; }

    // getters
    ThreadPool &      getThreadPool() { return *mup_thread_pool; }
    FileSystem &      getFileSystem() { return *mup_file_system; }
    const pt::ptree & getRootConfig() const { return m_root_config; }
    App &             getApp() { return *mup_app; }
    uint32_t          getFPS() const { return m_fps; }
};
}   // namespace evnt
#endif   // CORE_H
