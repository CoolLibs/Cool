// Start of [Block1]
// From https://entropymine.com/imageworsener/srgbformula/
float Cool_sRGB_to_linear_impl(float x)
{
    return x < 0.04045
               ? x / 12.92
               : pow((x + 0.055) / 1.055, 2.4);
}

float Cool_linear_to_sRGB_impl(float x)
{
    return x < 0.0031308
               ? x * 12.92
               : 1.055 * pow(x, 1. / 2.4) - 0.055;
}

vec3 Cool_sRGB_to_LinearRGB(vec3 srgb)
{
    srgb = saturate(srgb); // TODO(JF) Do we do this every time? Or shouldn't the clamp actually happen out the nodes' boundary? // EDIT: probably no, because the conversion requires values between 0 and 1 anyways
    return vec3(
        Cool_sRGB_to_linear_impl(srgb.x),
        Cool_sRGB_to_linear_impl(srgb.y),
        Cool_sRGB_to_linear_impl(srgb.z)
    );
}

vec3 Cool_LinearRGB_to_sRGB(vec3 rgb)
{
    rgb = saturate(rgb);
    return vec3(
        Cool_linear_to_sRGB_impl(rgb.x),
        Cool_linear_to_sRGB_impl(rgb.y),
        Cool_linear_to_sRGB_impl(rgb.z)
    );
}
// End of [Block1]

// Start of [Block2]
// From https://www.shadertoy.com/view/4syfRc

// LAB/RGB convertions - https://code.google.com/archive/p/flowabs/

vec3 Cool_LinearRGB_to_XYZ(vec3 c)
{
    return c * mat3(0.4124, 0.3576, 0.1805, 0.2126, 0.7152, 0.0722, 0.0193, 0.1192, 0.9505);
}

vec3 Cool_sRGB_to_XYZ(vec3 c)
{
    return Cool_LinearRGB_to_XYZ(Cool_sRGB_to_LinearRGB(c));
}

vec3 Cool_XYZ_to_CIELAB(vec3 c)
{
    vec3 n = c / vec3(0.95047, 1., 1.08883);
    vec3 v;
    v.x = (n.x > 0.008856) ? pow(n.x, 1.0 / 3.0) : (7.787 * n.x) + (16.0 / 116.0);
    v.y = (n.y > 0.008856) ? pow(n.y, 1.0 / 3.0) : (7.787 * n.y) + (16.0 / 116.0);
    v.z = (n.z > 0.008856) ? pow(n.z, 1.0 / 3.0) : (7.787 * n.z) + (16.0 / 116.0);
    vec3 lab = vec3((1.160 * v.y) - 0.16, 500.0 * (v.x - v.y), 200.0 * (v.y - v.z));
    return vec3(lab.x, 0.5 + 0.5 * (lab.y / 127.0), 0.5 + 0.5 * (lab.z / 127.0));
}

vec3 Cool_sRGB_to_CIELAB(vec3 c)
{
    return Cool_XYZ_to_CIELAB(Cool_sRGB_to_XYZ(c));
}

vec3 Cool_LinearRGB_to_CIELAB(vec3 c)
{
    return Cool_XYZ_to_CIELAB(Cool_LinearRGB_to_XYZ(c));
}

vec3 Cool_CIELAB_to_XYZ(vec3 c)
{
    float fy = (c.x + 16.0) / 116.0;
    float fx = c.y / 500.0 + fy;
    float fz = fy - c.z / 200.0;
    return vec3(
        0.95047 * ((fx > 0.206897) ? fx * fx * fx : (fx - 16.0 / 116.0) / 7.787),
        1.00000 * ((fy > 0.206897) ? fy * fy * fy : (fy - 16.0 / 116.0) / 7.787),
        1.08883 * ((fz > 0.206897) ? fz * fz * fz : (fz - 16.0 / 116.0) / 7.787)
    );
}

vec3 Cool_XYZ_to_LinearRGB(vec3 c)
{
    return c * mat3(3.2406, -1.5372, -0.4986, -0.9689, 1.8758, 0.0415, 0.0557, -0.2040, 1.0570);
}

vec3 Cool_XYZ_to_sRGB(vec3 c)
{
    return Cool_LinearRGB_to_sRGB(Cool_XYZ_to_LinearRGB(c));
}

vec3 Cool_CIELAB_to_sRGB(vec3 c)
{
    return Cool_XYZ_to_sRGB(Cool_CIELAB_to_XYZ(vec3(c.x, 2.0 * 1.27 * (c.y - 0.5), 2.0 * 1.27 * (c.z - 0.5))));
}

vec3 Cool_CIELAB_to_LinearRGB(vec3 c)
{
    return Cool_sRGB_to_LinearRGB(Cool_CIELAB_to_sRGB(c));
}
// End of [Block2]