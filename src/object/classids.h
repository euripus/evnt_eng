#ifndef CLASSIDS_H
#define CLASSIDS_H

#include <cstdint>

#define ClassName(name)              CLASS_##name
#define DefineClassID(name, classID) ClassName(name) = classID,

enum ClassIDType : int32_t
{
    DefineClassID(Undefined, -1) DefineClassID(Object, 1000) DefineClassID(Component, 1001)
        DefineClassID(Entity, 1002) DefineClassID(CameraComponent, 1003) DefineClassID(SpatialComponent, 1004)
            DefineClassID(LightComponent, 1005)

                LargestRuntimeClassID
};

// make sure people dont accidentally define classids in other files:
#undef DefineClassID

#endif   // CLASSIDS_H
