#ifndef SAMPLER_H
#define SAMPLER_H

#include "graphics_types.h"
#include <glm/glm.hpp>
#include <map>
#include <queue>

namespace evnt
{
class TextureUnitManager
{
public:
    static void    Initialize(int32_t availableUnits);
    static int32_t RefreshUnit(int32_t id);

private:
    inline static std::map<int32_t, int32_t> sm_available_units;
    inline static std::queue<int32_t>        sm_age_sort;
};

class ISampler
{
public:
    ISampler()          = default;
    virtual ~ISampler() = default;

    WrapMode getWrapMode(TextureChannel channel) const
    {
        return channel == TextureChannel::S ? m_s_wrap : channel == TextureChannel::T ? m_t_wrap : m_r_wrap;
    }

    MinificationFilter  getMinificationFilter() const { return m_min_filter; }
    MagnificationFilter getMagnificationFilter() const { return m_mag_filter; }
    bool                getCompareMode() const { return m_depth_compare; }
    ComparisonFunc      getComparisonFunc() const { return m_compare_func; }
    bool                getAnisotropicFiltering() const { return m_anisotropic_filtering; }
    glm::vec4           getBorderColor() const { return m_border_color; }

    virtual void    destroy()                                          = 0;
    virtual int32_t bindToTextureUnit()                                = 0;
    virtual void    setWrapMode(TextureChannel channel, WrapMode mode) = 0;
    virtual void    setMinificationFilter(MinificationFilter filter)   = 0;
    virtual void    setMagnificationFilter(MagnificationFilter filter) = 0;
    virtual void    setCompareMode(bool enabled)                       = 0;
    virtual void    setComparison(ComparisonFunc func)                 = 0;
    virtual void    setAnisotropicFiltering(bool enabled)              = 0;
    virtual void    setBorderColor(glm::vec4 color)                    = 0;

    uint32_t getID() const { return m_id; }

protected:
    WrapMode m_s_wrap;
    WrapMode m_t_wrap;
    WrapMode m_r_wrap;

    MinificationFilter  m_min_filter;
    MagnificationFilter m_mag_filter;

    bool           m_depth_compare;
    ComparisonFunc m_compare_func;

    bool      m_anisotropic_filtering;
    glm::vec4 m_border_color;

    uint32_t m_id;
};
}   // namespace evnt

#endif   // SAMPLER_H
