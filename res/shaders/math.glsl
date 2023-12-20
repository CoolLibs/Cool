const float TAU     = 6.283185307180;
const float PI      = 3.141592653590;
const float sqrt_3  = 1.73205;
const float FLT_MAX = 3.402823466e+38;
const float FLT_MIN = 1.175494351e-38;
const int   INT_MIN = -2147483648;
const int   INT_MAX = 2147483647;

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

int rand_xorshift(int seed)
{
    // Xorshift algorithm from George Marsaglia's paper
    seed ^= (seed << 13);
    seed ^= (seed >> 17);
    seed ^= (seed << 5);
    return seed;
}

float rand_xorshift_0_to_1(float seed)
{
    int seed_int = floatBitsToInt(seed);
    seed_int     = rand_xorshift(seed_int);
    float random = (float(seed_int) / float(INT_MAX));
    return random / 2 + 0.5;
}

vec3 hash_0_to_1_3D_to_3D(vec3 p)
{
    float xy = rand_xorshift_0_to_1(p.x + p.y);
    float yz = rand_xorshift_0_to_1(p.y + p.z);
    float zx = rand_xorshift_0_to_1(p.z + p.x);
    return vec3(
        rand_xorshift_0_to_1(xy * p.z),
        rand_xorshift_0_to_1(yz * p.x),
        rand_xorshift_0_to_1(zx * p.y)
    );
}

vec2 hash_0_to_1_1D_to_2D(float n)
{
    float x = rand_xorshift_0_to_1(n);
    float y = rand_xorshift_0_to_1(x);
    return vec2(x, y);
}

vec2 hash_0_to_1_2D_to_2D(vec2 p)
{
    float xy = rand_xorshift_0_to_1(p.x + p.y);
    return vec2(
        rand_xorshift_0_to_1(xy * p.x),
        rand_xorshift_0_to_1(xy * p.y)
    );
}

// Good for input value in [0,+10k]
float impl_base_noise(float seed1, float seed2)
{
    // From https://www.shadertoy.com/view/tlcBRl
    return (fract(seed1 + 12.34567 * fract(100. * (abs(seed1 * 0.91) + seed2 + 94.68) * fract((abs(seed2 * 0.41) + 45.46) * fract((abs(seed2) + 757.21) * fract(seed1 * 0.0171)))))) * 1.0038 - 0.00185;
}

float hash_0_to_1_2D_to_1D(vec2 p)
{
    vec2 s = hash_0_to_1_2D_to_2D(p);
    return rand_xorshift_0_to_1(s.x + s.y);
}

// Good for input value in [-100k,+100k]
float hash_0_to_1_3D_to_1D(vec3 p)
{
    vec3 s = hash_0_to_1_3D_to_3D(p);
    return rand_xorshift_0_to_1(s.x + s.y + s.z);
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