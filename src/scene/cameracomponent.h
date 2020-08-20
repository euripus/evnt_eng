#ifndef CAMERA_H
#define CAMERA_H

#include "../core/component.h"
#include "frustum.h"

namespace evnt
{
class CameraComponent : public Component
{
public:
    OBJECT_DEFINE(CameraComponent, Component)

    CameraComponent();
    ~CameraComponent() override = default;

    void setupViewParams(float fov, float aspect, float near_plane, float far_plane);
    void setProjectionMatrix(float * proj_mat);

    const Frustum &   getFrustum() { return m_frustum; }
    const glm::mat4 & getViewMat() { return m_view_mat; }
    const glm::mat4 & getProjMat() { return m_proj_mat; }
    const glm::vec3 & getAbsPos() { return m_abs_pos; }

    // called after SpatialComponent changed
    void postTransformUpdate(glm::mat4 const & new_trans);

private:
    glm::mat4  m_view_mat, m_proj_mat;
    Frustum    m_frustum;
    glm::vec3  m_abs_pos;
    glm::ivec2 m_vp_pos;
    glm::ivec2 m_vp_size;
    float      m_frust_left, m_frust_right, m_frust_bottom, m_frust_top, m_frust_near, m_frust_far;
    bool       m_orthographic;      // Perspective or orthographic frustum?
    bool       m_manual_proj_mat;   // Projection matrix manually set?

    // FrameGraph data
};

CameraComponent::CameraComponent() :
    m_view_mat{1.0f},
    m_proj_mat{1.0f},
    m_frustum{},
    m_abs_pos{0.0f, 0.0f, 0.0f},
    m_vp_pos{0, 0},
    m_vp_size{640, 480},
    m_frust_left{0.0f},
    m_frust_right{0.0f},
    m_frust_bottom{0.0f},
    m_frust_top{0.0f},
    m_frust_near{0.0f},
    m_frust_far{0.0f},
    m_orthographic{false},
    m_manual_proj_mat{false}
{}

}   // namespace evnt

#endif   // CAMERA_H
