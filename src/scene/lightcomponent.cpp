#include "lightcomponent.h"

namespace evnt
{
OBJECT_IMPLEMENT(LightComponent, Component)

// serialization support
void LightComponent::dump(int32_t indentLevel) const {}

void LightComponent::write(OutputMemoryStream & inMemoryStream, ObjectManager const & gmgr) const {}

void LightComponent::read(InputMemoryStream const & inMemoryStream, ObjectManager & gmgr) {}

void LightComponent::link(ObjectManager & gmgr, std::map<uint32_t, uint32_t> const & id_remap) {}
}   // namespace evnt
