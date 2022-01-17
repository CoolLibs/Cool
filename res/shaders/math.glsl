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

float hash_0_to_1(vec3 p)
{
    p = fract(p * .1031);
    p += dot(p, p.zyx + 31.32);
    return fract((p.x + p.y) * p.z);
}

#define saturate(v) clamp(v, 0., 1.)