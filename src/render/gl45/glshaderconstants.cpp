#include "glshaderconstants.h"
#include <GL/glew.h>

namespace evnt
{
void GLShaderConstants::bindConstants()
{
    for(std::pair<int32_t, std::pair<ITexture *, ISampler *>> lts : m_uniform_tex)
        glUniform1i(lts.first, lts.second.first->bindToSampler(*lts.second.second));
    for(std::pair<int32_t, Image *> lts : m_uniform_i)
        glUniform1i(lts.first, lts.second->bindToUnit());
    for(std::pair<int32_t, int32_t> lts : m_uniform1i)
        glUniform1i(lts.first, lts.second);
    for(std::pair<int32_t, float> lts : m_uniform1f)
        glUniform1f(lts.first, lts.second);
    for(std::pair<int32_t, glm::vec2> lts : m_uniform2f)
        glUniform2f(lts.first, lts.second.x, lts.second.y);
    for(std::pair<int32_t, glm::vec3> lts : m_uniform3f)
        glUniform3f(lts.first, lts.second.x, lts.second.y, lts.second.z);
    for(std::pair<int32_t, glm::vec4> lts : m_uniform4f)
        glUniform4f(lts.first, lts.second.x, lts.second.y, lts.second.z, lts.second.w);
    for(std::pair<int32_t, glm::mat3> lts : m_uniform3fv_mat)
        glUniformMatrix3fv(lts.first, 1, false, &lts.second[0][0]);
    for(std::pair<int32_t, glm::mat4> lts : m_uniform4fv_mat)
        glUniformMatrix4fv(lts.first, 1, false, &lts.second[0][0]);
}
}   // namespace evnt
