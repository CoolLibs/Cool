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
/*
 * HSLuv-C: Human-friendly HSL
 * <https://github.com/hsluv/hsluv-c>
 * <https://www.hsluv.org/>
 *
 * Copyright (c) 2015 Alexei Boronine (original idea, JavaScript implementation)
 * Copyright (c) 2015 Roger Tallada (Obj-C implementation)
 * Copyright (c) 2017 Martin Mitas (C implementation, based on Obj-C implementation)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/* for RGB */
static const glm::vec3 m[3] = {
    {3.24096994190452134377, -1.53738317757009345794, -0.49861076029300328366},
    {-0.96924363628087982613, 1.87596750150772066772, 0.04155505740717561247},
    {0.05563007969699360846, -0.20397695888897656435, 1.05697151424287856072}};

/* for XYZ */
static const glm::vec3 m_inv[3] = {
    {0.41239079926595948129, 0.35758433938387796373, 0.18048078840183428751},
    {0.21263900587151035754, 0.71516867876775592746, 0.07219231536073371500},
    {0.01933081871559185069, 0.11919477979462598791, 0.95053215224966058086}};

static const float ref_u = 0.19783000664283680764f;
static const float ref_v = 0.46831999493879100370f;

static const float kappa   = 903.29629629629629629630f;
static const float epsilon = 0.00885645167903563082f;

struct Bounds {
    float a;
    float b;
};

static void get_bounds(float l, Bounds bounds[6])
{
    float  tl   = l + 16.0f;
    float sub1 = (tl * tl * tl) / 1560896.0f;
    float sub2 = (sub1 > epsilon ? sub1 : (l / kappa));
    int   channel;
    int   t;

    for (channel = 0; channel < 3; channel++)
    {
        float m1 = m[channel].x;
        float m2 = m[channel].y;
        float m3 = m[channel].z;

        for (t = 0; t < 2; t++)
        {
            float top1   = (284517.f * m1 - 94839.f * m3) * sub2;
            float top2   = (838422.f * m3 + 769860.f * m2 + 731718.f * m1) * l * sub2 - 769860.f * t * l;
            float bottom = (632260.f * m3 - 126452.f * m2) * sub2 + 126452.f * t;

            bounds[channel * 2 + t].a = top1 / bottom;
            bounds[channel * 2 + t].b = top2 / bottom;
        }
    }
}

static float intersect_line_line(Bounds const& line1, Bounds const& line2)
{
    return (line1.b - line2.b) / (line2.a - line1.a);
}

static float dist_from_pole_squared(float x, float y)
{
    return x * x + y * y;
}

static float ray_length_until_intersect(float theta, Bounds const& line)
{
    return line.b / (sin(theta) - line.a * cos(theta));
}

static float max_safe_chroma_for_l(float l)
{
    float  min_len_squared = FLT_MAX;
    Bounds bounds[6];

    get_bounds(l, bounds);
    for (int i = 0; i < 6; i++)
    {
        float m1 = bounds[i].a;
        float b1 = bounds[i].b;
        /* x where line intersects with perpendicular running though (0, 0) */
        Bounds line2    = {-1.f / m1, 0.f};
        float  x        = intersect_line_line(bounds[i], line2);
        float  distance = dist_from_pole_squared(x, b1 + x * m1);

        if (distance < min_len_squared)
            min_len_squared = distance;
    }

    return std::sqrt(min_len_squared);
}

static float max_chroma_for_lh(float l, float h)
{
    float  min_len = FLT_MAX;
    float  hrad    = h * 0.01745329251994329577f; /* (2 * pi / 360) */
    Bounds bounds[6];
    int    i;

    get_bounds(l, bounds);
    for (i = 0; i < 6; i++)
    {
        float len = ray_length_until_intersect(hrad, bounds[i]);

        if (len >= 0 && len < min_len)
            min_len = len;
    }
    return min_len;
}

/* https://en.wikipedia.org/wiki/CIELUV
 * In these formulas, Yn refers to the reference white point. We are using
 * illuminant D65, so Yn (see refY in Maxima file) equals 1. The formula is
 * simplified accordingly.
 */
static float y2l(float y)
{
    if (y <= epsilon)
        return y * kappa;
    else
        return 116.f * std::cbrt(y) - 16.f;
}

static float l2y(float l)
{
    if (l <= 8.f)
    {
        return l / kappa;
    }
    else
    {
        float x = (l + 16.f) / 116.f;
        return x * x * x;
    }
}

static void xyz2luv(glm::vec3& in_out)
{
    float var_u = (4.f * in_out.x) / (in_out.x + (15.f * in_out.y) + (3.f * in_out.z));
    float var_v = (9.f * in_out.y) / (in_out.x + (15.f * in_out.y) + (3.f * in_out.z));
    float l     = y2l(in_out.y);
    float u     = 13.f * l * (var_u - ref_u);
    float v     = 13.f * l * (var_v - ref_v);

    in_out.x = l;
    if (l < 0.00000001f)
    {
        in_out.y = 0.f;
        in_out.z = 0.f;
    }
    else
    {
        in_out.y = u;
        in_out.z = v;
    }
}

static void luv2xyz(glm::vec3& in_out)
{
    if (in_out.x <= 0.00000001f)
    {
        /* Black will create a divide-by-zero error. */
        in_out.x = 0.f;
        in_out.y = 0.f;
        in_out.z = 0.f;
        return;
    }

    float var_u = in_out.y / (13.f * in_out.x) + ref_u;
    float var_v = in_out.z / (13.f * in_out.x) + ref_v;
    float y     = l2y(in_out.x);
    float x     = -(9.f * y * var_u) / ((var_u - 4.f) * var_v - var_u * var_v);
    float z     = (9.f * y - (15.f * var_v * y) - (var_v * x)) / (3.f * var_v);
    in_out.x    = x;
    in_out.y    = y;
    in_out.z    = z;
}

static void luv2lch(glm::vec3& in_out)
{
    float l = in_out.x;
    float u = in_out.y;
    float v = in_out.z;
    float h;
    float c = std::sqrt(u * u + v * v);

    /* Grays: disambiguate hue */
    if (c < 0.00000001f)
    {
        h = 0.f;
    }
    else
    {
        h = atan2(v, u) * 57.29577951308232087680f; /* (180 / pi) */
        if (h < 0.f)
            h += 360.f;
    }

    in_out.x = l;
    in_out.y = c;
    in_out.z = h;
}

static void lch2luv(glm::vec3& in_out)
{
    float hrad = in_out.z * 0.01745329251994329577f; /* (pi / 180.0) */
    float u    = std::cos(hrad) * in_out.y;
    float v    = std::sin(hrad) * in_out.y;

    in_out.y = u;
    in_out.z = v;
}

static void hsluv2lch(glm::vec3& in_out)
{
    float h = in_out.x;
    float s = in_out.y;
    float l = in_out.z;
    float c;

    /* White and black: disambiguate chroma */
    if (l > 99.9999999f || l < 0.00000001f)
        c = 0.f;
    else
        c = max_chroma_for_lh(l, h) / 100.f * s;

    /* Grays: disambiguate hue */
    if (s < 0.00000001f)
        h = 0.f;

    in_out.x = l;
    in_out.y = c;
    in_out.z = h;
}

static void lch2hsluv(glm::vec3& in_out)
{
    float l = in_out.x;
    float c = in_out.y;
    float h = in_out.z;
    float s;

    /* White and black: disambiguate saturation */
    if (l > 99.9999999f || l < 0.00000001f)
        s = 0.f;
    else
        s = c / max_chroma_for_lh(l, h) * 100.f;

    /* Grays: disambiguate hue */
    if (c < 0.00000001f)
        h = 0.f;

    in_out.x = h;
    in_out.y = s;
    in_out.z = l;
}

auto XYZ_from_HSLuv(glm::vec3 const& col) -> glm::vec3
{
    glm::vec3 tmp = col * glm::vec3{360., 100.f, 100.f};

    hsluv2lch(tmp);
    lch2luv(tmp);
    luv2xyz(tmp);

    return tmp;
}

auto HSLuv_from_XYZ(glm::vec3 const& col) -> glm::vec3
{
    glm::vec3 tmp = col;

    xyz2luv(tmp);
    luv2lch(tmp);
    lch2hsluv(tmp);

    return tmp / glm::vec3{360., 100.f, 100.f};
}

// SRGB
auto HSLuv_from_sRGB(glm::vec3 const& c) -> glm::vec3
{
    return HSLuv_from_XYZ(XYZ_from_sRGB(c));
}

auto sRGB_from_HSLuv(glm::vec3 const& c) -> glm::vec3
{
    return sRGB_from_XYZ(XYZ_from_HSLuv(c));
}

// LinearRGB
auto HSLuv_from_LinearRGB(glm::vec3 const& c) -> glm::vec3
{
    return HSLuv_from_XYZ(XYZ_from_LinearRGB(c));
}

auto LinearRGB_from_HSLuv(glm::vec3 const& c) -> glm::vec3
{
    return LinearRGB_from_XYZ(XYZ_from_HSLuv(c));
}

// CIELAB
auto HSLuv_from_CIELAB(glm::vec3 const& c) -> glm::vec3
{
    return HSLuv_from_XYZ(XYZ_from_CIELAB(c));
}

auto CIELAB_from_HSLuv(glm::vec3 const& c) -> glm::vec3
{
    return CIELAB_from_XYZ(XYZ_from_HSLuv(c));
}

// End of [Block3]
} // namespace Cool
