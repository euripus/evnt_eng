#include "demostate.h"
#include "../app/app.h"

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
}
}   // namespace evnt
