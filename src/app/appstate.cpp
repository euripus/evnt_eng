#include "appstate.h"
#include "app.h"

namespace evnt
{
void null_state::update()
{
    terminate();
}

void null_state::terminate()
{
    m_owner.stop();
}
}   // namespace evnt
