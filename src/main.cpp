#include "core/core.h"

#include <codecvt>
#include <iomanip>
#include <iostream>
#include <locale>

#ifdef WIN32
#include <shellapi.h>
#include <windows.h>
#endif

int utf8_main(int argc, char * argv[])
{
    try
    {
        evnt::Core::startUp();

        // add AppStates
        // and set  start state
        evnt::Core::instance().getApp().setStartState(evnt::Core::instance().getApp().getEndStateID());

        if(!evnt::Core::instance().init(argc, argv))
        {
            std::cout << "Errror! Application couldn't be initialized" << std::endl;
            return 1;
        }

        evnt::Core::instance().enterMainLoop();

        evnt::Core::shutDown();
    }
    catch(const std::exception & ex)
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
    int     win_argc;
    char ** win_argv;

    wchar_t ** wargv = CommandLineToArgvW(GetCommandLineW(), &win_argc);
    win_argv         = new char *[win_argc + 1];

    for(int i = 0; i < win_argc; i++)
    {
        int n       = WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, NULL, 0, NULL, NULL);
        win_argv[i] = new char[n];
        WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, win_argv[i], n, NULL, NULL);
    }
    LocalFree(wargv);

    auto ret = utf8_main(win_argc, win_argv);

    for(int i = 0; i < win_argc; i++)
    {
        delete[] win_argv[i];
    }
    delete win_argv;

    return ret;
#else
    return utf8_main(argc, argv);
#endif
}
