#include "conversions.h"
#include <algorithm>
#include "Cool/ColorSpaces/conversions.h"
#include "Cool/Math/constants.h"
#include "glm/fwd.hpp"

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
        (1.16f * v.y) - 0.16f,
        5.f * (v.x - v.y),
        2.f * (v.y - v.z),
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
    float const fy = (c.x + 0.16f) / 1.16f;
    float const fx = c.y / 5.f + fy;
    float const fz = fy - c.z / 2.f;

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

// Start of [Block3]
// From https://www.hsluv.org/
struct Bounds {
    float a;
    float b;
};

static const float kappa   = 903.29629629629629629630f;
static const float epsilon = 0.00885645167903563082f;

static const float ref_u = 0.19783000664283680764f;
static const float ref_v = 0.46831999493879100370f;

static const std::array<glm::vec3, 3> m{glm::vec3(3.24096994190452134377f, -1.53738317757009345794f, -0.49861076029300328366f), glm::vec3(-0.96924363628087982613f, 1.87596750150772066772f, 0.04155505740717561247f), glm::vec3(0.05563007969699360846f, -0.20397695888897656435f, 1.05697151424287856072f)};

static auto y2l(float y) -> float
{
    if(y <= epsilon)
        return y * kappa;
    else
        return 116. * cbrt(y) - 16.;
}

static auto get_bounds(float l, Cool::Bounds bounds[6]) -> void
{
    float tl   = l + 16.f;
    float sub1 = (tl * tl * tl) / 1560896.0f;
    float sub2 = (sub1 > Cool::epsilon ? sub1 : (l / Cool::kappa));
    int   t;

    for (int channel = 0; channel < 3; channel++)
    {
        float m1 = Cool::m[channel].x;
        float m2 = Cool::m[channel].y;
        float m3 = Cool::m[channel].z;

        for (t = 0; t < 2; t++)
        {
            float top1   = (284517.0f * m1 - 94839.0f * m3) * sub2;
            float top2   = (838422.0f * m3 + 769860.0f * m2 + 731718.0f * m1) * l * sub2 - 769860.0f * t * l;
            float bottom = (632260.0f * m3 - 126452.0f * m2) * sub2 + 126452.0f * t;

            bounds[channel * 2 + t].a = top1 / bottom;
            bounds[channel * 2 + t].b = top2 / bottom;
        }
    }
}

static auto ray_length_until_intersect(float theta, const Bounds* line) -> float
{
    return line->b / (sin(theta) - line->a * cos(theta));
}

static auto max_chroma_for_lh(float l, float h) -> float
{
    float        min_len = FLT_MAX;
    float        hrad    = h * 0.01745329251994329577f; /* (2 * pi / 360) */
    Cool::Bounds bounds[6];
    get_bounds(l, bounds);
    for (int i = 0; i < 6; i++)
    {
        float len = Cool::ray_length_until_intersect(hrad, &bounds[i]);

        if (len >= 0 && len < min_len)
            min_len = len;
    }
    return min_len;
}

auto HSLuv_from_XYZ(glm::vec3 c) -> glm::vec3
{
    c *= 100.;
    // XYZ -> LUV
    float var_u = (4.f * c.x) / (c.x + (15.f * c.y) + (3.f * c.z));
    float var_v = (9.f * c.y) / (c.x + (15.f * c.y) + (3.f * c.z));
    float l     = y2l(c.y);
    float u     = 13.f * l * (var_u - Cool::ref_u);
    float v     = 13.f * l * (var_v - Cool::ref_v);

    c.x = l;
    if (l < 0.00000001f)
    {
        c.y = 0.f;
        c.z = 0.f;
    }

    else
    {
        c.y = u;
        c.z = v;
    }

    // LUV -> lch
    float h;
    float c2 = std::sqrt(u * u + v * v);

    /* Grays: disambiguate hue */
    if (c2 < 0.00000001f)
    {
        h = 0.f;
    }
    else
    {
        h = std::atan2(v, u) * 57.29577951308232087680f; /* (180 / pi) */
        if (h < 0.f)
            h += 360.f;
    }

    // lch -> HSLuv
    float s;

    /* White and black: disambiguate saturation */
    if (l > 0.9999999f || l < 0.00000001f)
        s = 0.f;
    else
        s = c2 / Cool::max_chroma_for_lh(l, h) * 100.f;

    /* Grays: disambiguate hue */
    if (c2 < 0.00000001f)
        h = 0.f;

    return glm::vec3(h / 360., s / 100., l / 100.);
}

auto XYZ_from_HSLuv(glm::vec3 c) -> glm::vec3
{
    // HSLuv -> lch
    float h = c.x;
    float s = c.y;
    float l = c.z;
    float c2;

    /* White and black: disambiguate chroma */
    if (l > 99.9999999f || l < 0.00000001f)
        c2 = 0.f;
    else
        c2 = Cool::max_chroma_for_lh(l, h);

    /* Grays: disambiguate hue */
    if (s < 0.00000001f)
    {
        h = 0.f;
    }

    // lch -> LUV
    float hrad = h * tau;
    float u    = cos(hrad) * c2;
    float v    = sin(hrad) * c2;

    // LUV -> XYZ
    if (l <= 0.00000001f) return glm::vec3(0.f);

    float var_u = u / (13.f * l) + Cool::ref_u;
    float var_v = v / (13.f * l) + Cool::ref_v;
    float y     = (l <= 8.f) ? l / Cool::kappa : ((l + 16.f) / 116.f) * ((l + 16.f) / 116.f) * ((l + 16.f) / 116.f);
    float x     = -(9.f * y * var_u) / ((var_u - 4.f) * var_v - var_u * var_v);
    float z     = (9.f * y - (15.f * var_v * y) - (var_v * x)) / (3.f * var_v);

    return glm::vec3(x, y, z) / 100.f;
}

// SRGB
auto HSLuv_from_sRGB(glm::vec3 c) -> glm::vec3
{
    return HSLuv_from_XYZ(XYZ_from_sRGB(c));
}

auto sRGB_from_HSLuv(glm::vec3 c) -> glm::vec3
{
    return sRGB_from_XYZ(XYZ_from_HSLuv(c));
}

// LinearRGB
auto HSLuv_from_LinearRGB(glm::vec3 c) -> glm::vec3
{
    return HSLuv_from_XYZ(XYZ_from_LinearRGB(c));
}

auto LinearRGB_from_HSLuv(glm::vec3 c) -> glm::vec3
{
    return LinearRGB_from_XYZ(XYZ_from_HSLuv(c));
}

// CIELAB
auto HSLuv_from_CIELAB(glm::vec3 c) -> glm::vec3
{
    return HSLuv_from_XYZ(XYZ_from_CIELAB(c));
}

auto CIELAB_from_HSLuv(glm::vec3 c) -> glm::vec3
{
    return CIELAB_from_XYZ(XYZ_from_HSLuv(c));
}

// End of [Block3]
} // namespace Cool
