#include "conversions.h"
#include <algorithm>

namespace Cool {

static auto saturate(glm::vec3 const& v) -> glm::vec3
{
    return {
        std::clamp(v.x, 0.f, 1.f),
        std::clamp(v.y, 0.f, 1.f),
        std::clamp(v.z, 0.f, 1.f),
    };
}

// Start of [Block1]
// From https://entropymine.com/imageworsener/srgbformula/

static auto LinearRGB_from_sRGB_impl(float x) -> float
{
    return x < 0.04045f
               ? x / 12.92f
               : std::pow((x + 0.055f) / 1.055f, 2.4f);
}

static auto sRGB_from_LinearRGB_impl(float x) -> float
{
    return x < 0.0031308f
               ? x * 12.92f
               : 1.055f * std::pow(x, 1.f / 2.4f) - 0.055f;
}

auto LinearRGB_from_sRGB(glm::vec3 srgb) -> glm::vec3
{
    srgb = saturate(srgb);
    return {
        LinearRGB_from_sRGB_impl(srgb.x),
        LinearRGB_from_sRGB_impl(srgb.y),
        LinearRGB_from_sRGB_impl(srgb.z),
    };
}

auto sRGB_from_LinearRGB(glm::vec3 rgb) -> glm::vec3
{
    rgb = saturate(rgb);
    return {
        sRGB_from_LinearRGB_impl(rgb.x),
        sRGB_from_LinearRGB_impl(rgb.y),
        sRGB_from_LinearRGB_impl(rgb.z),
    };
}
// End of [Block1]

// Start of [Block2]
// From http://www.easyrgb.com/en/math.php

auto XYZ_from_LinearRGB(glm::vec3 const& c) -> glm::vec3
{
    return c * glm::mat3(0.4124f, 0.3576f, 0.1805f, 0.2126f, 0.7152f, 0.0722f, 0.0193f, 0.1192f, 0.9505f);
}

auto XYZ_from_sRGB(glm::vec3 const& c) -> glm::vec3
{
    return XYZ_from_LinearRGB(LinearRGB_from_sRGB(c));
}

auto CIELAB_from_XYZ(glm::vec3 const& c) -> glm::vec3
{
    glm::vec3 const n = c / glm::vec3(0.95047f, 1.f, 1.08883f);
    glm::vec3 const v{
        (n.x > 0.008856) ? std::pow(n.x, 1.f / 3.f) : (7.787f * n.x) + (16.f / 116.f),
        (n.y > 0.008856) ? std::pow(n.y, 1.f / 3.f) : (7.787f * n.y) + (16.f / 116.f),
        (n.z > 0.008856) ? std::pow(n.z, 1.f / 3.f) : (7.787f * n.z) + (16.f / 116.f),
    };
    return {
        (116.f * v.y) - 16.f,
        500.f * (v.x - v.y),
        200.f * (v.y - v.z),
    };
}

auto CIELAB_from_sRGB(glm::vec3 const& c) -> glm::vec3
{
    return CIELAB_from_XYZ(XYZ_from_sRGB(c));
}

auto CIELAB_from_LinearRGB(glm::vec3 const& c) -> glm::vec3
{
    return CIELAB_from_XYZ(XYZ_from_LinearRGB(c));
}

auto XYZ_from_CIELAB(glm::vec3 const& c) -> glm::vec3
{
    float const fy = (c.x + 16.f) / 116.f;
    float const fx = c.y / 500.f + fy;
    float const fz = fy - c.z / 200.f;

    float const fx3 = fx * fx * fx;
    float const fy3 = fy * fy * fy;
    float const fz3 = fz * fz * fz;
    return {
        0.95047f * ((fx3 > 0.008856f) ? fx3 : (fx - 16.f / 116.f) / 7.787f),
        1.00000f * ((fy3 > 0.008856f) ? fy3 : (fy - 16.f / 116.f) / 7.787f),
        1.08883f * ((fz3 > 0.008856f) ? fz3 : (fz - 16.f / 116.f) / 7.787f),
    };
}

auto LinearRGB_from_XYZ(glm::vec3 const& c) -> glm::vec3
{
    return c * glm::mat3(3.2406f, -1.5372f, -0.4986f, -0.9689f, 1.8758f, 0.0415f, 0.0557f, -0.2040f, 1.0570f);
}

auto sRGB_from_XYZ(glm::vec3 const& c) -> glm::vec3
{
    return sRGB_from_LinearRGB(LinearRGB_from_XYZ(c));
}

auto sRGB_from_CIELAB(glm::vec3 const& c) -> glm::vec3
{
    return sRGB_from_XYZ(XYZ_from_CIELAB(c));
}

auto LinearRGB_from_CIELAB(glm::vec3 const& c) -> glm::vec3
{
    return LinearRGB_from_sRGB(sRGB_from_CIELAB(c));
}
// End of [Block2]
} // namespace Cool