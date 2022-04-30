#include "./demo/demostate.h"
#include "core/core.h"

#include <iostream>

#ifdef WIN32
#include <shellapi.h>
#include <windows.h>
#endif

int32_t utf8_main(int32_t argc, char * argv[])
{
    try
    {
        evnt::Core::startUp();

        // add AppStates
        // and set  start state
        auto start_id =
            evnt::Core::instance().getApp().addAppState<evnt::DemoState>(evnt::Core::instance().getApp());
        evnt::Core::instance().getApp().setStartState(start_id);

        if(!evnt::Core::instance().appInit(argc, argv))
        {
            std::cout << "Errror! Application couldn't be initialized" << std::endl;
            return 1;
        }

        evnt::Core::instance().enterMainLoop();

        evnt::Core::shutDown();
    }
    catch(std::exception const & ex)
    {
        std::cout << "ERROR: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

// https://stackoverflow.com/questions/148403/utf8-to-from-wide-char-conversion-in-stl/14809553
int main(int argc, char * argv[])
{
#ifdef WIN32
    int32_t win_argc;
    char ** win_argv;

    wchar_t ** wargv = CommandLineToArgvW(GetCommandLineW(), &win_argc);
    win_argv         = new char *[win_argc + 1];

    for(int32_t i = 0; i < win_argc; i++)
    {
        int32_t n   = WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, NULL, 0, NULL, NULL);
        win_argv[i] = new char[n];
        WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, win_argv[i], n, NULL, NULL);
    }
    LocalFree(wargv);

    auto ret = utf8_main(win_argc, win_argv);

    for(int32_t i = 0; i < win_argc; i++)
    {
        delete[] win_argv[i];
    }
    delete[] win_argv;

    return ret;
#else
    return utf8_main(argc, argv);
#endif
}
