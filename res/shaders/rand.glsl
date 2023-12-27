// Inspired by https://stackoverflow.com/a/17479300

const float UINT_MAX = 4294967295.;

uint uint_hash_1D(uint seed)
{
    // A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
    seed += (seed << 10u);
    seed ^= (seed >> 6u);
    seed += (seed << 3u);
    seed ^= (seed >> 11u);
    seed += (seed << 15u);
    return seed;
}
uint uint_hash_2D(uvec2 seed)
{
    return uint_hash_1D(seed.x ^ uint_hash_1D(seed.y));
}
uint uint_hash_3D(uvec3 seed)
{
    return uint_hash_1D(seed.x ^ uint_hash_2D(seed.yz));
}
uint uint_hash_4D(uvec4 seed)
{
    return uint_hash_1D(seed.x ^ uint_hash_3D(seed.yzw));
}

float hash_0_to_1_1D_to_1D(float seed)
{
    return float(uint_hash_1D(floatBitsToUint(seed))) / UINT_MAX;
}
float hash_0_to_1_2D_to_1D(vec2 seed)
{
    return float(uint_hash_2D(floatBitsToUint(seed))) / UINT_MAX;
}
float hash_0_to_1_3D_to_1D(vec3 seed)
{
    return float(uint_hash_3D(floatBitsToUint(seed))) / UINT_MAX;
}
float hash_0_to_1_4D_to_1D(vec4 seed)
{
    return float(uint_hash_4D(floatBitsToUint(seed))) / UINT_MAX;
}

vec2 hash_0_to_1_1D_to_2D(float seed)
{
    float rand1 = hash_0_to_1_1D_to_1D(seed);
    float rand2 = hash_0_to_1_1D_to_1D(rand1);
    return vec2(rand1, rand2);
}
vec3 hash_0_to_1_1D_to_3D(float seed)
{
    float rand1 = hash_0_to_1_1D_to_1D(seed);
    float rand2 = hash_0_to_1_1D_to_1D(rand1);
    float rand3 = hash_0_to_1_1D_to_1D(rand2);
    return vec3(rand1, rand2, rand3);
}
vec4 hash_0_to_1_1D_to_4D(float seed)
{
    float rand1 = hash_0_to_1_1D_to_1D(seed);
    float rand2 = hash_0_to_1_1D_to_1D(rand1);
    float rand3 = hash_0_to_1_1D_to_1D(rand2);
    float rand4 = hash_0_to_1_1D_to_1D(rand3);
    return vec4(rand1, rand2, rand3, rand4);
}

vec2 hash_0_to_1_2D_to_2D(vec2 p)
{
    return vec2(
        hash_0_to_1_2D_to_1D(p.xy),
        hash_0_to_1_2D_to_1D(p.yx)
    );
}
vec3 hash_0_to_1_3D_to_3D(vec3 p)
{
    return vec3(
        hash_0_to_1_3D_to_1D(p.xyz),
        hash_0_to_1_3D_to_1D(p.yxz),
        hash_0_to_1_3D_to_1D(p.zxy)
    );
}
vec4 hash_0_to_1_4D_to_4D(vec4 p)
{
    return vec4(
        hash_0_to_1_4D_to_1D(p.xyzw),
        hash_0_to_1_4D_to_1D(p.yxzw),
        hash_0_to_1_4D_to_1D(p.zxyw),
        hash_0_to_1_4D_to_1D(p.wxyz)
    );
}