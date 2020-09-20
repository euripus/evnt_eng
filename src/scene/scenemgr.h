#ifndef SCENEMGR_H
#define SCENEMGR_H

#include "../object/gameobject.h"
//#include "cameracomponent.h"
//#include "scenecomponent.h"

namespace evnt
{
class SceneMgr
{
public:
    SceneMgr();
    virtual ~SceneMgr() = default;

    //virtual void getRenderList(CameraComponent const & cam, std::vector<SpatialComponent *> & out_list) const = 0;
};
}   // namespace evnt
#endif   // SCENEMGR_H
