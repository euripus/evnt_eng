#ifndef SHADERCONSTANTS_H
#define SHADERCONSTANTS_H

#include "texture.h"
#include <glm/glm.hpp>
#include <vector>

namespace evnt
{
class ShaderConstants
{
public:
    ShaderConstants();
    virtual ~ShaderConstants() = default;

    void addConstant(int32_t location, ITexture * texture, ISampler * sampler);
    void addConstant(int32_t location, Image * image);
    void addConstant(int32_t location, int32_t i);
    void addConstant(int32_t location, float f);
    void addConstant(int32_t location, glm::vec2 v2);
    void addConstant(int32_t location, glm::vec3 v3);
    void addConstant(int32_t location, glm::vec4 v4);
    void addConstant(int32_t location, glm::mat3 m3);
    void addConstant(int32_t location, glm::mat4 m4);

    virtual void bindConstants() = 0;

    std::vector<std::pair<int32_t, std::pair<ITexture *, ISampler *>>> getTextureBindings() const
    {
        return m_uniform_tex;
    }
    std::vector<std::pair<int32_t, Image *>>   getImageBindings() const { return m_uniform_i; }
    std::vector<std::pair<int32_t, int32_t>>   getIntBindings() const { return m_uniform1i; }
    std::vector<std::pair<int32_t, float>>     getFloatBindings() const { return m_uniform1f; }
    std::vector<std::pair<int32_t, glm::vec2>> getVec2Bindings() const { return m_uniform2f; }
    std::vector<std::pair<int32_t, glm::vec3>> getVec3Bindings() const { return m_uniform3f; }
    std::vector<std::pair<int32_t, glm::vec4>> getVec4Bindings() const { return m_uniform4f; }
    std::vector<std::pair<int32_t, glm::mat3>> getMat3Bindings() const { return m_uniform3fv_mat; }
    std::vector<std::pair<int32_t, glm::mat4>> getMat4Bindings() const { return m_uniform4fv_mat; }

protected:
    std::vector<std::pair<int32_t, std::pair<ITexture *, ISampler *>>> m_uniform_tex;
    std::vector<std::pair<int32_t, Image *>>                           m_uniform_i;
    std::vector<std::pair<int32_t, int32_t>>                           m_uniform1i;
    std::vector<std::pair<int32_t, float>>                             m_uniform1f;
    std::vector<std::pair<int32_t, glm::vec2>>                         m_uniform2f;
    std::vector<std::pair<int32_t, glm::vec3>>                         m_uniform3f;
    std::vector<std::pair<int32_t, glm::vec4>>                         m_uniform4f;
    std::vector<std::pair<int32_t, glm::mat3>>                         m_uniform3fv_mat;
    std::vector<std::pair<int32_t, glm::mat4>>                         m_uniform4fv_mat;
};
}   // namespace evnt
#endif   // SHADERCONSTANTS_H
