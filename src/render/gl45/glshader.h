#ifndef GLSHADER_H
#define GLSHADER_H

#include "../shader.h"
#include <set>

namespace evnt
{
class GLShader : public IShader
{
public:
    GLShader() = default;
    GLShader(std::string src, std::set<std::string> defines);
    ~GLShader() override;

    void destroy() override;
};
}   // namespace evnt

#endif   // GLSHADER_H
