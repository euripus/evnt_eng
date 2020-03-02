#ifndef GLSHADERCONSTANTS_H
#define GLSHADERCONSTANTS_H

#include "../shaderconstants.h"

namespace evnt
{
class GLShaderConstants : public ShaderConstants
{
public:
    GLShaderConstants()           = default;
    ~GLShaderConstants() override = default;

    void bindConstants() override;
};
}   // namespace evnt
#endif   // GLSHADERCONSTANTS_H
