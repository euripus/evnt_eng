#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "../object/component.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace evnt
{
class LightComponent : public Component
{
public:
    OBJECT_DEFINE

    enum class LightType
    {
        ltAmbient,
        ltDirectional,
        ltPoint,
        ltSpot,
        ltMax
    };

    LightComponent()           = default;
    ~LightComponent() override = default;

    LightType m_type = {LightType::ltMax};

    // The colors of the light.
    glm::vec4 m_ambient  = {0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 m_diffuse  = {0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 m_specular = {0.0f, 0.0f, 0.0f, 1.0f};

    // Attenuation is typically specified as a modulator
    //     m = 1/(C + L*d + Q*d*d)
    // where C is the constant coefficient, L is the linear coefficient,
    // Q is the quadratic coefficient, and d is the distance from the light
    // position to the vertex position.  To allow for a linear adjustment of
    // intensity, the choice is to use instead
    //     m = I/(C + L*d + Q*d*d)
    // where I is an intensity factor.
    float m_constant  = 1.0f;
    float m_linear    = 0.0f;
    float m_quadratic = 0.0f;
    float m_intensity = 1.0f;

    // Parameters for spot lights.  The cone angle must be in radians and
    // should satisfy 0 < Angle <= pi.
    float m_angle     = glm::pi<float>();
    float m_cos_angle = -1.0f;
    float m_sin_angle = 0.0f;
    float m_exponent  = 1.0f;

    glm::vec3 m_position  = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_direction = {0.0f, 0.0f, -1.0f};
};
}   // namespace evnt

#endif   // LIGHTCOMPONENT_H
