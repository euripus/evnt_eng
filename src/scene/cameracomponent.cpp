#include "cameracomponent.h"
#include <cstring>

namespace evnt
{
OBJECT_IMPLEMENT(CameraComponent, Component)

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

void CameraComponent::setupViewParams(float fov, float aspect, float near_plane, float far_plane)
{
    float ymax = near_plane * glm::tan(glm::radians(fov / 2));
    float xmax = ymax * aspect;

    m_frust_left   = -xmax;
    m_frust_right  = xmax;
    m_frust_bottom = -ymax;
    m_frust_top    = ymax;
    m_frust_near   = near_plane;
    m_frust_far    = far_plane;

    // setting view params implicitly disables the manual projection matrix
    m_manual_proj_mat = false;
}

void CameraComponent::setProjectionMatrix(float * proj_mat)
{
    std::memcpy(&m_proj_mat[0], proj_mat, 16 * sizeof(float));
    m_manual_proj_mat = true;
}

void CameraComponent::postTransformUpdate(glm::mat4 const & new_trans)
{
    // Get position
    m_abs_pos = glm::vec3(new_trans[3][0], new_trans[3][1], new_trans[3][2]);

    // Calculate view matrix
    m_view_mat = glm::inverse(new_trans);

    // Calculate projection matrix if not using a manually set one
    if(!m_manual_proj_mat)
    {
        if(!m_orthographic)
            m_proj_mat = glm::frustum(m_frust_left, m_frust_right, m_frust_bottom, m_frust_top, m_frust_near,
                                      m_frust_far);
        else
            m_proj_mat = glm::ortho(m_frust_left, m_frust_right, m_frust_bottom, m_frust_top, m_frust_near,
                                    m_frust_far);
    }

    // Update frustum
    m_frustum.buildViewFrustum(m_view_mat, m_proj_mat);
}

// serialization support
void CameraComponent::dump(int32_t indentLevel) const {}

void CameraComponent::write(OutputMemoryStream & inMemoryStream, ObjectManager const & gmgr) const {}

void CameraComponent::read(InputMemoryStream const & inMemoryStream, ObjectManager & gmgr) {}

void CameraComponent::link(ObjectManager & gmgr, std::map<uint32_t, uint32_t> const & id_remap) {}
}   // namespace evnt
