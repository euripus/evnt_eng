#include "shaderconstants.h"

namespace evnt
{
template<class T>
static void FindDuplicateAndReplace(std::vector<std::pair<int32_t, T>> & vec, int32_t location, T data)
{
    size_t index = 0;
    for(std::pair<int32_t, T> pair : vec)
    {
        if(pair.first == location)
            break;
        ++index;
    }
    if(index == vec.size())
        vec.push_back(std::make_pair(location, data));
    else
        vec[index] = std::make_pair(location, data);
}

void ShaderConstants::addConstant(int32_t location, ITexture * texture, ISampler * sampler)
{
    FindDuplicateAndReplace(m_uniform_tex, location, std::make_pair(texture, sampler));
}

void ShaderConstants::addConstant(int32_t location, Image * image)
{
    FindDuplicateAndReplace(m_uniform_i, location, image);
}

void ShaderConstants::addConstant(int32_t location, int32_t i)
{
    FindDuplicateAndReplace(m_uniform1i, location, i);
}

void ShaderConstants::addConstant(int32_t location, float f)
{
    FindDuplicateAndReplace(m_uniform1f, location, f);
}

void ShaderConstants::addConstant(int32_t location, glm::vec2 v2)
{
    FindDuplicateAndReplace(m_uniform2f, location, v2);
}

void ShaderConstants::addConstant(int32_t location, glm::vec3 v3)
{
    FindDuplicateAndReplace(m_uniform3f, location, v3);
}

void ShaderConstants::addConstant(int32_t location, glm::vec4 v4)
{
    FindDuplicateAndReplace(m_uniform4f, location, v4);
}

void ShaderConstants::addConstant(int32_t location, glm::mat3 m3)
{
    FindDuplicateAndReplace(m_uniform3fv_mat, location, m3);
}

void ShaderConstants::addConstant(int32_t location, glm::mat4 m4)
{
    FindDuplicateAndReplace(m_uniform4fv_mat, location, m4);
}
}   // namespace evnt
