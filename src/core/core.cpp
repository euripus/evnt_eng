#include "core.h"
#include "../log/log.h"
#include <boost/property_tree/json_parser.hpp>
#include <chrono>

namespace evnt
{
static std::chrono::steady_clock::time_point app_start{std::chrono::steady_clock::now()};

int64_t GetMilisecFromStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - app_start)
        .count();
}

Core::Core()
{
    m_thread_pool  = std::make_unique<ThreadPool>();
    m_event_system = std::make_unique<EventSystem>(*m_thread_pool);

    // http://techgate.fr/boost-property-tree/
    // Load the config.json file in this ptree
    pt::read_json("config.json", m_root_config);

    m_file_system =
        std::make_unique<FileSystem>(m_root_config.get<std::string>("FileSystem.RootPathRelative"));

    Log::SeverityLevel sl =
        Log::ConvertStrToSeverity(m_root_config.get<std::string>("Logging.SeverityLevelFilter"));
    Log::Output ot = Log::ConvertStrToOutput(m_root_config.get<std::string>("Logging.Output"));
    Log::BoostLog::InitBoostLog(m_root_config.get<std::string>("Logging.FileName"), ot);
    Log::BoostLog::SetSeverityFilter(sl);

    registerEvent<evExit>();
    addFunctor<evExit>(std::bind(&Core::exit, this));
}

void Core::enterMainLoop()
{
    if(m_running)
        return;
    else
        m_running = true;

    // http://www.opengl-tutorial.org/miscellaneous/an-fps-counter/
    uint32_t previous = GetMilisecFromStart();
    uint32_t lag      = 0;
    uint32_t nbFrames = 0;
    uint32_t lastTime = previous;

    // https://gameprogrammingpatterns.com/game-loop.html
    while(m_running && getApp().running())
    {
        uint32_t current = GetMilisecFromStart();
        uint32_t elapsed = current - previous;
        previous         = current;
        lag += elapsed;
        nbFrames++;
        if(current - lastTime >= 1000)
        {   // If last save was more than 1sec ago
            // save and reset
            m_fps    = nbFrames;
            nbFrames = 0;
            lastTime += 1000;
        }

        getApp().processInput();
        while(lag >= MS_PER_UPDATE)
        {
            getApp().update();   // Logic physics et cetera
            lag -= MS_PER_UPDATE;
        }

        getApp().onFrame();   // render
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<uint32_t>(MS_PER_UPDATE - lag)));
    }

    getApp().terminate();
}

}   // namespace evnt
