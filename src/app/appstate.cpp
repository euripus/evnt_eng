#include "appstate.h"
#include "app.h"

namespace evnt
{
void end_state::update()
{
    m_owner.stop();
}
}   // namespace evnt
