#ifndef APP_H
#define APP_H

namespace evnt
{
class App
{
public:
    App()          = default;
    virtual ~App() = default;

    // in ovverided functions need to be called parent first
    virtual bool init(int argc, char * argv[]) { return true; }
    virtual void terminate() {}
    virtual void update() {}
    virtual void onFrame() {}   // call render() for  main window

    virtual bool running() const { return true; }
};
}   // namespace evnt
#endif   // APP_H
