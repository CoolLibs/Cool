// Inspired by https://stackoverflow.com/a/17479300
#include "_COOL_RES_/shaders/math.glsl"

uint uint_hash_1D_to_1D(uint seed)
{
    // A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
    seed += (seed << 10u);
    seed ^= (seed >> 6u);
    seed += (seed << 3u);
    seed ^= (seed >> 11u);
    seed += (seed << 15u);
    return seed;
}
uint uint_hash_2D_to_1D(uvec2 seed)
{
    return uint_hash_1D_to_1D(seed.x ^ uint_hash_1D_to_1D(seed.y));
}
uint uint_hash_3D_to_1D(uvec3 seed)
{
    return uint_hash_1D_to_1D(seed.x ^ uint_hash_2D_to_1D(seed.yz));
}
uint uint_hash_4D_to_1D(uvec4 seed)
{
    return uint_hash_1D_to_1D(seed.x ^ uint_hash_3D_to_1D(seed.yzw));
}

uvec2 uint_hash_1D_to_2D(uint seed)
{
    uint rand1 = uint_hash_1D_to_1D(seed);
    uint rand2 = uint_hash_1D_to_1D(rand1);
    return uvec2(
        rand1,
        rand2
    );
}
uvec3 uint_hash_1D_to_3D(uint seed)
{
    uint rand1 = uint_hash_1D_to_1D(seed);
    uint rand2 = uint_hash_1D_to_1D(rand1);
    uint rand3 = uint_hash_1D_to_1D(rand2);
    return uvec3(
        rand1,
        rand2,
        rand3
    );
}
uvec4 uint_hash_1D_to_4D(uint seed)
{
    uint rand1 = uint_hash_1D_to_1D(seed);
    uint rand2 = uint_hash_1D_to_1D(rand1);
    uint rand3 = uint_hash_1D_to_1D(rand2);
    uint rand4 = uint_hash_1D_to_1D(rand3);
    return uvec4(
        rand1,
        rand2,
        rand3,
        rand4
    );
}

uvec2 uint_hash_2D_to_2D(uvec2 seed)
{
    return uint_hash_1D_to_2D(uint_hash_2D_to_1D(seed));
}
uvec3 uint_hash_2D_to_3D(uvec2 seed)
{
    return uint_hash_1D_to_3D(uint_hash_2D_to_1D(seed));
}
uvec4 uint_hash_2D_to_4D(uvec2 seed)
{
    return uint_hash_1D_to_4D(uint_hash_2D_to_1D(seed));
}
uvec2 uint_hash_3D_to_2D(uvec3 seed)
{
    return uint_hash_1D_to_2D(uint_hash_3D_to_1D(seed));
}
uvec3 uint_hash_3D_to_3D(uvec3 seed)
{
    return uint_hash_1D_to_3D(uint_hash_3D_to_1D(seed));
}
uvec4 uint_hash_3D_to_4D(uvec3 seed)
{
    return uint_hash_1D_to_4D(uint_hash_3D_to_1D(seed));
}
uvec2 uint_hash_4D_to_2D(uvec4 seed)
{
    return uint_hash_1D_to_2D(uint_hash_4D_to_1D(seed));
}
uvec3 uint_hash_4D_to_3D(uvec4 seed)
{
    return uint_hash_1D_to_3D(uint_hash_4D_to_1D(seed));
}
uvec4 uint_hash_4D_to_4D(uvec4 seed)
{
    return uint_hash_1D_to_4D(uint_hash_4D_to_1D(seed));
}

float float_0_to_1_from_uint(uint x)
{
    const float UINT_MAX_AS_FLOAT = 4294967295.;
    return float(x) / UINT_MAX_AS_FLOAT;
}
vec2 vec2_0_to_1_from_uint(uvec2 x)
{
    const float UINT_MAX_AS_FLOAT = 4294967295.;
    return vec2(x) / UINT_MAX_AS_FLOAT;
}
vec3 vec3_0_to_1_from_uint(uvec3 x)
{
    const float UINT_MAX_AS_FLOAT = 4294967295.;
    return vec3(x) / UINT_MAX_AS_FLOAT;
}
vec4 vec4_0_to_1_from_uint(uvec4 x)
{
    const float UINT_MAX_AS_FLOAT = 4294967295.;
    return vec4(x) / UINT_MAX_AS_FLOAT;
}

float hash_0_to_1_1D_to_1D(float seed)
{
    return float_0_to_1_from_uint(uint_hash_1D_to_1D(floatBitsToUint(seed)));
}
float hash_0_to_1_2D_to_1D(vec2 seed)
{
    return float_0_to_1_from_uint(uint_hash_2D_to_1D(floatBitsToUint(seed)));
}
float hash_0_to_1_3D_to_1D(vec3 seed)
{
    return float_0_to_1_from_uint(uint_hash_3D_to_1D(floatBitsToUint(seed)));
}
float hash_0_to_1_4D_to_1D(vec4 seed)
{
    return float_0_to_1_from_uint(uint_hash_4D_to_1D(floatBitsToUint(seed)));
}
vec2 hash_0_to_1_1D_to_2D(float seed)
{
    return vec2_0_to_1_from_uint(uint_hash_1D_to_2D(floatBitsToUint(seed)));
}
vec3 hash_0_to_1_1D_to_3D(float seed)
{
    return vec3_0_to_1_from_uint(uint_hash_1D_to_3D(floatBitsToUint(seed)));
}
vec4 hash_0_to_1_1D_to_4D(float seed)
{
    return vec4_0_to_1_from_uint(uint_hash_1D_to_4D(floatBitsToUint(seed)));
}
vec2 hash_0_to_1_2D_to_2D(vec2 seed)
{
    return vec2_0_to_1_from_uint(uint_hash_2D_to_2D(floatBitsToUint(seed)));
}
vec3 hash_0_to_1_2D_to_3D(vec2 seed)
{
    return vec3_0_to_1_from_uint(uint_hash_2D_to_3D(floatBitsToUint(seed)));
}
vec4 hash_0_to_1_2D_to_4D(vec2 seed)
{
    return vec4_0_to_1_from_uint(uint_hash_2D_to_4D(floatBitsToUint(seed)));
}
vec2 hash_0_to_1_3D_to_2D(vec3 seed)
{
    return vec2_0_to_1_from_uint(uint_hash_3D_to_2D(floatBitsToUint(seed)));
}
vec3 hash_0_to_1_3D_to_3D(vec3 seed)
{
    return vec3_0_to_1_from_uint(uint_hash_3D_to_3D(floatBitsToUint(seed)));
}
vec4 hash_0_to_1_3D_to_4D(vec3 seed)
{
    return vec4_0_to_1_from_uint(uint_hash_3D_to_4D(floatBitsToUint(seed)));
}
vec2 hash_0_to_1_4D_to_2D(vec4 seed)
{
    return vec2_0_to_1_from_uint(uint_hash_4D_to_2D(floatBitsToUint(seed)));
}
vec3 hash_0_to_1_4D_to_3D(vec4 seed)
{
    return vec3_0_to_1_from_uint(uint_hash_4D_to_3D(floatBitsToUint(seed)));
}
vec4 hash_0_to_1_4D_to_4D(vec4 seed)
{
    return vec4_0_to_1_from_uint(uint_hash_4D_to_4D(floatBitsToUint(seed)));
}

vec2 random_point_on_disk(float seed)
{
    vec2  random    = hash_0_to_1_1D_to_2D(seed);
    float theta     = 2 * PI * random.x;
    float r         = sqrt(random.y);
    vec2  direction = vec2(cos(theta), sin(theta));
    return r * direction;
}

vec3 random_point_on_sphere(float seed)
{
    vec3  random    = hash_0_to_1_1D_to_3D(seed);
    float theta     = 2 * PI * random.x;
    float ry        = random.y * 2 - 1;
    float ry2       = sqrt(1 - ry * ry);
    float r         = pow(random.z, 1. / 3.);
    vec3  direction = vec3(cos(theta) * ry2, sin(theta) * ry2, ry);
    return r * direction;
}
