const float TAU    = 6.283185307180;
const float PI     = 3.141592653590;
const float sqrt_3 = 1.73205;

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

float hash_0_to_1_3D_to_1D(vec3 p)
{
    return fract(sin(dot(p, vec3(12.9898, 78.233, 74.7) * 2.0)) * 43758.5453);
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

vec3 hash_minus_1_to_1_3D_to_3D(vec3 p)
{
    return -1.0 + 2.0 * hash_0_to_1_3D_to_3D(p);
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

float hash_0_to_1_2D_to_1D(vec2 p)
{
    return fract(sin(dot(p, vec2(12.9898, 78.233) * 2.0)) * 43758.5453);
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

vec2 polar_from_vec2(vec2 pos)
{
    float r = length(pos);
    return vec2(r, pos.y > 0 ? acos(dot(pos, vec2(1, 0)) / r) : 2 * PI - acos(dot(pos, vec2(1, 0)) / r));
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

#define FLT_MAX 3.402823466e+38