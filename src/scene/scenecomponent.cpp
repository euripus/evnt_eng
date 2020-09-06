#include "scenecomponent.h"

namespace evnt
{
OBJECT_IMPLEMENT(SpatialComponent, Component)

void SpatialComponent::updateTree(uint32_t ms_delta, bool initiator)
{
    updateWorldData(ms_delta);
    updateWorldBound();

    if(initiator)
    {
        propagateBoundToRoot();
    }
}

void SpatialComponent::updateWorldData(uint32_t ms_delta)
{
    // Update any controllers associated with this object.
    // updateControllers(ms_delta);

    // Update world transforms.
    if(m_parent)
    {
        m_world_transform = m_parent->m_world_transform * m_local_transform;
    }
    else
    {
        m_world_transform = m_local_transform;
    }

    // update children
    for(auto & ch : m_children)
    {
        ch->updateTree(ms_delta, false);
    }
}

void SpatialComponent::updateWorldBound()
{
    m_bbox.transform(m_world_transform);

    for(const auto & ch : m_children)
    {
        m_bbox.expandBy(ch->m_bbox);
    }
}

void SpatialComponent::propagateBoundToRoot()
{
    if(m_parent)
    {
        m_parent->updateWorldBound();
        m_parent->propagateBoundToRoot();
    }
}

int32_t SpatialComponent::attachChild(SpatialComponent * child)
{
    if(!child)
    {
        // assertion(false, "You cannot attach null children to a node.\n");
        return -1;
    }

    if(child->getParent())
    {
        // assertion(false, "The child already has a parent.\n");
        return -1;
    }

    child->setParent(this);

    // Append the child to the array.
    const int32_t num_children = static_cast<int32_t>(m_children.size());
    m_children.push_back(child);
    return num_children;
}

int32_t SpatialComponent::detachChild(SpatialComponent * child)
{
    if(child)
    {
        auto iter = std::begin(m_children);
        auto end  = std::end(m_children);
        for(int32_t i = 0; iter != end; ++iter, ++i)
        {
            if(*iter == child)
            {
                (*iter)->setParent(nullptr);
                m_children.erase(iter);
                return i;
            }
        }
    }
    return -1;
}

SpatialComponent * SpatialComponent::detachChildAt(int32_t i)
{
    if(0 <= i && i < static_cast<int32_t>(m_children.size()))
    {
        SpatialComponent * child = m_children[i];
        if(child)
        {
            child->setParent(nullptr);
            m_children.erase(m_children.begin() + i);
        }
        return child;
    }
    return 0;
}

SpatialComponent * SpatialComponent::getChild(int32_t i)
{
    if(0 <= i && i < static_cast<int32_t>(m_children.size()))
    {
        return m_children[i];
    }
    return nullptr;
}

// serialization support
void SpatialComponent::dump(int32_t indentLevel) const {}

void SpatialComponent::write(OutputMemoryStream & inMemoryStream, const ObjectManager & gmgr) const {}

void SpatialComponent::read(const InputMemoryStream & inMemoryStream, ObjectManager & gmgr) {}

void SpatialComponent::link(ObjectManager & gmgr, const std::map<uint32_t, uint32_t> & id_remap) {}
}   // namespace evnt
