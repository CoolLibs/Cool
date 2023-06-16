const float TAU     = 6.283185307180;
const float PI      = 3.141592653590;
const float sqrt_3  = 1.73205;
const float FLT_MAX = 3.402823466e+38;
const float FLT_MIN = 1.175494351e-38;

// See https://www.iquilezles.org/www/articles/smin/smin.htm
float smooth_min_polynomial(float a, float b, float smoothing)
{
    float h = max(smoothing - abs(a - b), 0.0) / smoothing;
    return min(a, b) - h * h * smoothing * 0.25;
}

// We use the fact that `smax(a, b) = - smin(-a, -b)` and reuse the formula above for smin
float smooth_max_polynomial(float a, float b, float smoothing)
{
    float h = max(smoothing - abs(a - b), 0.0) / smoothing;
    return max(a, b) + h * h * smoothing * 0.25;
}

float ndot(vec2 a, vec2 b)
{
    return a.x * b.x - a.y * b.y;
}

float length_squared(vec2 p)
{
    return dot(p, p);
}

float length_squared(vec3 p)
{
    return dot(p, p);
}

float length_squared(vec4 p)
{
    return dot(p, p);
}

vec3 hash_0_to_1_3D_to_3D(vec3 p)
{
    p = vec3(
        dot(p, vec3(127.1, 311.7, 74.7)),
        dot(p, vec3(269.5, 183.3, 246.1)),
        dot(p, vec3(113.5, 271.9, 124.6))
    );

    return fract(sin(p) * 43758.5453123);
}

vec2 hash_0_to_1_1D_to_2D(float n)
{
    return fract(sin(vec2(n, n + 1.0)) * vec2(43758.5453123, 22578.1459123));
}

vec2 hash_0_to_1_2D_to_2D(vec2 p)
{
    vec3 a = fract(p.xyx * vec3(123.34, 234.34, 345.65));
    a += dot(a, a + 34.45);
    return fract(vec2(a.x * a.y, a.y * a.z));
}

// Good for input value in [0,+10k]
float impl_base_noise(float seed1, float seed2)
{
    // From https://www.shadertoy.com/view/tlcBRl
    return (fract(seed1 + 12.34567 * fract(100. * (abs(seed1 * 0.91) + seed2 + 94.68) * fract((abs(seed2 * 0.41) + 45.46) * fract((abs(seed2) + 757.21) * fract(seed1 * 0.0171)))))) * 1.0038 - 0.00185;
}

// Good for input value in [-100k,+100k]
float hash_0_to_1_2D_to_1D(vec2 p)
{
    // From https://www.shadertoy.com/view/tlcBRl
    float buff1 = abs(p.x + 100.94) + 1000.;
    float buff2 = abs(p.y + 100.73) + 1000.;
    buff1       = buff1 * fract(buff2 * fract(buff1 * fract(buff2 * 0.63)));
    buff2       = buff2 * fract(buff2 * fract(buff1 + buff2 * fract(p.x * 0.79)));
    buff1       = impl_base_noise(buff1, buff2);
    return buff1 * 1.0038 - 0.00185;
}

// Good for input value in [-100k,+100k]
float hash_0_to_1_3D_to_1D(vec3 p)
{
    // From https://www.shadertoy.com/view/tlcBRl
    float buff1 = abs(p.x + 100.81) + 1000.3;
    float buff2 = abs(p.y + 100.45) + 1000.2;
    float buff3 = abs(impl_base_noise(p.x, p.y) + p.z) + 1000.1;
    buff1       = buff3 * fract(buff2 * fract(buff1 * fract(buff2 * 0.146)));
    buff2       = buff2 * fract(buff2 * fract(buff1 + buff2 * fract(buff3 * 0.52)));
    buff1       = impl_base_noise(buff1, buff2);
    return buff1;
}

// Good for input value in [-100k,+100k], high accuracy
float hash_0_to_1_3D_to_1D_high_accuracy(vec3 p)
{
    // From https://www.shadertoy.com/view/tlcBRl
    float buff1 = abs(p.x + 100.813) + 1000.314;
    float buff2 = abs(p.y + 100.453) + 1000.213;
    float buff3 = abs(impl_base_noise(buff2, buff1) + p.z) + 1000.17;
    buff1       = buff3 * fract(buff2 * fract(buff1 * fract(buff2 * 0.14619)));
    buff2       = buff2 * fract(buff2 * fract(buff1 + buff2 * fract(buff3 * 0.5215)));
    buff1       = hash_0_to_1_3D_to_1D(vec3(impl_base_noise(p.y, buff1), impl_base_noise(p.x, buff2), impl_base_noise(p.z, buff3)));
    return buff1;
}

// Good for input value in [-100k,+100k], high accuracy
float hash_0_to_1_2D_to_1D_high_accuracy(vec2 p)
{
    return hash_0_to_1_3D_to_1D_high_accuracy(vec3(p.xy, 0.));
}

mat2 rotation_2D(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return mat2(
        +c, -s,
        +s, +c
    );
}

vec3 rotation_around_axis(vec3 p, vec3 ax, float ro)
{
    // from https://suricrasia.online/demoscene/functions/
    return mix(dot(ax, p) * ax, p, cos(ro)) + cross(ax, p) * sin(ro);
}

vec2 vec2_from_polar(float radius, float angle)
{
    return radius * vec2(cos(angle), sin(angle));
}

float Cool_angle(vec2 v)
{
    return v.x != 0.
               ? atan(v.y, v.x)
           : v.y > 0.
               ? PI / 2.
               : -PI / 2.;
}

vec2 Cool_polar_coordinates(vec2 v)
{
    return vec2(length(v), Cool_angle(v));
}

vec2 complex_product(vec2 a, vec2 b)
{
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

vec2 complex_division(vec2 n, vec2 d)
{
    return vec2(n.x * d.x + n.y * d.y, n.y * d.x - n.x * d.y) / (d.x * d.x + d.y * d.y);
}

#define saturate(v) clamp(v, 0., 1.)