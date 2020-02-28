#ifndef GLSAMPLER_H
#define GLSAMPLER_H

#include "../sampler.h"

namespace evnt
{
class GLSampler : public ISampler
{
public:
    GLSampler();
    ~GLSampler() override;

    void    destroy() override;
    int32_t bindToTextureUnit() override;
    void    setWrapMode(TextureChannel channel, WrapMode mode) override;
    void    setMinificationFilter(MinificationFilter filter) override;
    void    setMagnificationFilter(MagnificationFilter filter) override;
    void    setCompareMode(bool enabled) override;
    void    setComparison(ComparisonFunc func) override;
    void    setAnisotropicFiltering(bool enabled) override;
    void    setBorderColor(glm::vec4 color) override;
};
}   // namespace evnt

#endif   // GLSAMPLER_H
