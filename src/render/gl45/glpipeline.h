#ifndef GLPIPELINE_H
#define GLPIPELINE_H

#include "../pipeline.h"

namespace evnt
{
class GLPipeline : public Pipeline
{
public:
    GLPipeline();

    void bindPipeline() override;
};
}   // namespace evnt
#endif   // GLPIPELINE_H
