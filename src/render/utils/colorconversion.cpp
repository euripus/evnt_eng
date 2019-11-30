#include "colorconversion.h"
#include <array>

namespace evnt
{
namespace
{
    class LinearToSRGBMap
    {
    public:
        LinearToSRGBMap() noexcept
        {
            for(uint32_t i = 0; i < m_to_SRBG.size(); ++i)
            {
                m_to_SRBG[i] = LinearToSRGB(static_cast<float>(i) / 255.f);
            }
        }

        float operator[](uint8_t x) const { return m_to_SRBG[x]; }

    private:
        std::array<float, 256> m_to_SRBG;
    };

    class SRGBToLinearMap
    {
    public:
        SRGBToLinearMap() noexcept
        {
            for(uint32_t i = 0; i < m_to_linear.size(); ++i)
            {
                m_to_linear[i] = SRGBToLinear(static_cast<float>(i) / 255.f);
            }
        }

        float operator[](uint8_t x) const { return m_to_linear[x]; }

    private:
        std::array<float, 256> m_to_linear;
    };
}   // namespace

float LinearToSRGB(uint8_t x)
{
    static const LinearToSRGBMap map;
    return map[x];
}

float SRGBToLinear(uint8_t x)
{
    static const SRGBToLinearMap map;
    return map[x];
}
}   // namespace evnt
