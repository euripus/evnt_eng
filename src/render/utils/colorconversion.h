#ifndef COLORCONVERSION_H
#define COLORCONVERSION_H

#include <stdint.h>
#include <cmath>

namespace evnt
{
// https://en.wikipedia.org/wiki/SRGB
inline float LinearToSRGB(float x)
{
    return x <= 0.0031308 ? x * 12.92f : 1.055f * std::pow(x, 1.f / 2.4f) - 0.055f;
}

inline float SRGBToLinear(float x)
{
    return x <= 0.04045f ? x / 12.92f : std::pow((x + 0.055f) / 1.055f, 2.4f);
}

float LinearToSRGB(uint8_t x);
float SRGBToLinear(uint8_t x);

inline float FastLinearToSRGB(float x)
{
    return x < 0.0031308f ? 12.92f * x : 1.13005f * sqrtf(std::abs(x - 0.00228f)) - 0.13448f * x + 0.005719f;
}

inline float FastSRGBToLinear(float x)
{
    // http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
    return x * (x * (x * 0.305306011f + 0.682171111f) + 0.012522878f);
}

}   // namespace evnt

#endif   // COLORCONVERSION_H
