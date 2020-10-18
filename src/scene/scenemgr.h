#ifndef SCENEMGR_H
#define SCENEMGR_H

#include "../object/gameobject.h"
#include "cameracomponent.h"
#include "scenecomponent.h"

namespace evnt
{
class SceneMgr
{
public:
    enum class SortOrder
    {
        Back_To_Front,
        Front_To_Back
    };

    SceneMgr();
    virtual ~SceneMgr() = default;

    virtual void updateLists(Frustum const & cam_frustum, Frustum const * frust2, bool render_queue_update,
                             bool light_queue_update)                       = 0;
    virtual std::vector<PObjHandle> const & getRenderList() const           = 0;
    virtual std::vector<PObjHandle> const & getLightList() const            = 0;
    virtual void                            sortRenderList(SortOrder order) = 0;
};
}   // namespace evnt
#endif   // SCENEMGR_H
