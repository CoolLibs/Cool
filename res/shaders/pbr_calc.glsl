float G1V(float NdotV, float k)
{
    return 1.0 / (NdotV * (1.0 - k) + k);
}

float fresnel(vec3 normal, float metallic)
{
    float NdotV = clamp(dot(normal, vec3(0.0, 0.0, -1.0)), 0.0, 1.0);
    NdotV       = pow(1.0 - NdotV, 5.0);
    return metallic + (1.0 - metallic) * (NdotV);
}

float Fresnel1Term(float LdotH, float metallic)
{
    float LdotH5 = 1.0 - LdotH;
    LdotH5       = LdotH5 * LdotH5 * LdotH5 * LdotH5 * LdotH5;
    return metallic + (1.0 - metallic) * (LdotH5);
}

float VisibilityTerm(float roughtness, float NdotV, float NdotL)
{
    float k = (roughtness * roughtness) / 2;
    return G1V(NdotL, k) * G1V(NdotV, k);
}

float GeoTerm(float roughtness, float NdotH)
{
    float r4 = roughtness * roughtness * roughtness * roughtness;
    float pi = 3.14159;
    float d  = NdotH * NdotH * (r4 - 1.0) + 1.0;
    return r4 / (pi * d * d);
}

float specular(vec3 normal, vec3 lightdir, float roughtness, float metallic)
{
    vec3  v     = vec3(0.0, 0.0, -1.0);
    vec3  h     = normalize(v + lightdir);
    float NdotL = clamp(dot(normal, lightdir), 0.0, 1.0);
    float NdotV = clamp(dot(normal, v), 0.0, 1.0);
    float NdotH = clamp(dot(normal, h), 0.0, 1.0);
    float LdotH = clamp(dot(lightdir, h), 0.0, 1.0);

    return NdotL * GeoTerm(roughtness, NdotH) * Fresnel1Term(LdotH, metallic) * VisibilityTerm(roughtness, NdotV, NdotL);
}