#include "sampler.h"

namespace evnt
{
void TextureUnitManager::Initialize(int32_t availableUnits)
{
    for(int32_t i = 0; i < availableUnits; ++i)
    {
        sm_available_units.insert({i - availableUnits, i});
        sm_age_sort.push(i - availableUnits);
    }
}

int32_t TextureUnitManager::RefreshUnit(int32_t id)
{
    if(sm_available_units.find(id) != sm_available_units.end())
        return sm_available_units[id];
    else
    {
        int32_t oldId = sm_age_sort.front();
        sm_age_sort.pop();
        int32_t unit = sm_available_units[oldId];
        sm_available_units.erase(oldId);
        sm_available_units.insert({id, unit});
        sm_age_sort.push(id);
        return unit;
    }
}
}   // namespace evnt
