#ifndef SCENE_H
#define SCENE_H

#include "../object/component.h"
#include "AABB.h"

namespace evnt
{
struct SpatialComponent : public Component
{
    OBJECT_DEFINE

    // Culling parameters.
    enum class CullingMode
    {
        // Determine visibility state by comparing the world bounding volume
        // to culling planes.
        cull_dynamic,
        // Force the object to be culled.  If a Node is culled, its entire
        // subtree is culled.
        cull_always,
        // Never cull the object.  If a Node is never culled, its entire
        // subtree is never culled.
        cull_never
    };

    glm::mat4 m_local_transform;
    glm::mat4 m_world_transform;   // Transformation matrices

    AABB m_bbox;   // AABB in world space

    CullingMode m_culling;

    SpatialComponent *              m_parent;
    std::vector<SpatialComponent *> m_children;   // Child nodes

    SpatialComponent();
    ~SpatialComponent() override = default;

    SpatialComponent * getParent() { return m_parent; }
    void               setParent(SpatialComponent * parent) { m_parent = parent; }

    // Update of geometric state and controllers.  The function computes world
    // transformations on the downward pass of the scene graph traversal and
    // world bounding volumes on the upward pass of the traversal.
    void updateTree(uint32_t ms_delta = 0, bool initiator = true);
    void updateWorldData(uint32_t ms_delta);
    void updateWorldBound();
    void propagateBoundToRoot();

    // This is the current number of elements in the child array.
    int32_t getNumChildren() const { return static_cast<int32_t>(m_children.size()); }
    // Attach a child to this node.  If the function succeeds, the return
    // value is the index i of the array where the child was stored, in which
    // case 0 <= i < GetNumChildren().
    // The function fails when 'child' is null or when 'child' already has a
    // parent, in which case the return value is -1.  The nodes form a tree,
    // not a more general directed acyclic graph.  A consequence is that a
    // node cannot have more than one parent.
    int32_t attachChild(SpatialComponent * child);
    // Detach a child from this node.  If the 'child' is non-null and in the
    // array, the return value is the index in the array that had stored the
    // child.  Otherwise, the function returns -1.
    int32_t detachChild(SpatialComponent * child);
    // Detach a child from this node.  If 0 <= i < GetNumChildren(), the
    // return value is the child at index i; otherwise, the function returns
    // null.
    SpatialComponent * detachChildAt(int32_t i);
    // Get the child at the specified index.  If 0 <= i < GetNumChildren(),
    // the function succeeds and returns the child at that index.
    // If i is out of range, the function returns null.
    SpatialComponent * getChild(int32_t i);

    // culling
};
}   // namespace evnt

#endif   // SCENE_H
