#include "demostate.h"
#include "../app/app.h"
#include "../core/exception.h"

namespace evnt
{
void DemoState::update()
{
    // input update
    auto const & input = m_owner.getMainWindow().getInput();
    if(input.getKeyPressed() == Keyboard::Key_Escape)
    {
        m_owner.setNextState(m_owner.getEndStateID());
    }
    else if(input.getKeyPressed() == Keyboard::Key_F1)
    {
        m_fullscreen = !m_fullscreen;

        m_owner.getMainWindow().fullscreen(m_fullscreen);
    }
}
}   // namespace evnt
