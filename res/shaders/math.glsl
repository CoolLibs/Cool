const float TAU = 6.283185307180;
const float PI  = 3.141592653590;

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

float length_squared(vec3 p)
{
    return dot(p, p);
}

float hash_0_to_1(vec3 p)
{
    p = fract(p * .1031);
    p += dot(p, p.zyx + 31.32);
    return fract((p.x + p.y) * p.z);
}

vec3 hash_minus_1_to_1(vec3 p)
{
    p = vec3(dot(p, vec3(127.1, 311.7, 74.7)),
             dot(p, vec3(269.5, 183.3, 246.1)),
             dot(p, vec3(113.5, 271.9, 124.6)));

    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
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

mat2 rotation_2D(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return mat2(c, -s,
                s, c);
}

vec3 rotation_around_axis(vec3 p, vec3 ax, float ro)
{
    // from https://suricrasia.online/demoscene/functions/
    return mix(dot(ax, p) * ax, p, cos(ro)) + cross(ax, p) * sin(ro);
}

#define saturate(v) clamp(v, 0., 1.)