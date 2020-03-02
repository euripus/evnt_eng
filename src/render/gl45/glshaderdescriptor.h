#ifndef GLSHADERDESCRIPTOR_H
#define GLSHADERDESCRIPTOR_H

#include "../shaderdescriptor.h"

namespace evnt
{
class GLShaderDescriptor : public ShaderDescriptor
{
public:
    GLShaderDescriptor()           = default;
    ~GLShaderDescriptor() override = default;

    void bindDescriptor() override;
};
}   // namespace evnt

#endif   // GLSHADERDESCRIPTOR_H
