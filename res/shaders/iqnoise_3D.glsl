/// Taken from https://www.shadertoy.com/view/NtlSDs

// hash by Hugo Elias
float _fn3D_hash(int n)
{
    n = (n << 13) ^ n;
    n = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return 1.0 - float(n) * (1.0 / 1073741824.0);
}

float iqnoise_3D(vec3 p)
{
    ivec3 ip = ivec3(floor(p));
    vec3  fp = fract(p);

    vec3 w = fp * fp * (3.0 - 2.0 * fp);

    int n = ip.x + ip.y * 57 + 113 * ip.z;

    // clang-format off
    return mix(mix(mix(_fn3D_hash(n + (0 + 57 * 0 + 113 * 0)),
                       _fn3D_hash(n + (1 + 57 * 0 + 113 * 0)), w.x),
                   mix(_fn3D_hash(n + (0 + 57 * 1 + 113 * 0)),
                       _fn3D_hash(n + (1 + 57 * 1 + 113 * 0)), w.x),
                   w.y),
               mix(mix(_fn3D_hash(n + (0 + 57 * 0 + 113 * 1)),
                       _fn3D_hash(n + (1 + 57 * 0 + 113 * 1)), w.x),
                   mix(_fn3D_hash(n + (0 + 57 * 1 + 113 * 1)),
                       _fn3D_hash(n + (1 + 57 * 1 + 113 * 1)), w.x),
                   w.y),
               w.z);
    // clang-format on
}