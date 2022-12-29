// Start of [Block1]
// From https://entropymine.com/imageworsener/srgbformula/
float Cool_LinearRGB_from_sRGB_impl(float x)
{
    return x < 0.04045
               ? x / 12.92
               : pow((x + 0.055) / 1.055, 2.4);
}

float Cool_sRGB_from_LinearRGB_impl(float x)
{
    return x < 0.0031308
               ? x * 12.92
               : 1.055 * pow(x, 1. / 2.4) - 0.055;
}

vec3 Cool_LinearRGB_from_sRGB(vec3 srgb)
{
    srgb = saturate(srgb);
    return vec3(
        Cool_LinearRGB_from_sRGB_impl(srgb.x),
        Cool_LinearRGB_from_sRGB_impl(srgb.y),
        Cool_LinearRGB_from_sRGB_impl(srgb.z)
    );
}

vec3 Cool_sRGB_from_LinearRGB(vec3 rgb)
{
    rgb = saturate(rgb);
    return vec3(
        Cool_sRGB_from_LinearRGB_impl(rgb.x),
        Cool_sRGB_from_LinearRGB_impl(rgb.y),
        Cool_sRGB_from_LinearRGB_impl(rgb.z)
    );
}
// End of [Block1]

// Start of [Block2]
// From http://www.easyrgb.com/en/math.php

vec3 Cool_XYZ_from_LinearRGB(vec3 c)
{
    return c * mat3(0.4124, 0.3576, 0.1805, 0.2126, 0.7152, 0.0722, 0.0193, 0.1192, 0.9505);
}

vec3 Cool_XYZ_from_sRGB(vec3 c)
{
    return Cool_XYZ_from_LinearRGB(Cool_LinearRGB_from_sRGB(c));
}

vec3 Cool_CIELAB_from_XYZ(vec3 c)
{
    vec3 n = c / vec3(0.95047, 1., 1.08883);
    vec3 v;
    v.x = (n.x > 0.008856) ? pow(n.x, 1. / 3.) : (7.787 * n.x) + (16. / 116.);
    v.y = (n.y > 0.008856) ? pow(n.y, 1. / 3.) : (7.787 * n.y) + (16. / 116.);
    v.z = (n.z > 0.008856) ? pow(n.z, 1. / 3.) : (7.787 * n.z) + (16. / 116.);
    return vec3((116. * v.y) - 16., 500. * (v.x - v.y), 200. * (v.y - v.z));
}

vec3 Cool_CIELAB_from_sRGB(vec3 c)
{
    return Cool_CIELAB_from_XYZ(Cool_XYZ_from_sRGB(c));
}

vec3 Cool_CIELAB_from_LinearRGB(vec3 c)
{
    return Cool_CIELAB_from_XYZ(Cool_XYZ_from_LinearRGB(c));
}

vec3 Cool_XYZ_from_CIELAB(vec3 c)
{
    float fy = (c.x + 16.) / 116.;
    float fx = c.y / 500. + fy;
    float fz = fy - c.z / 200.;

    float fx3 = fx * fx * fx;
    float fy3 = fy * fy * fy;
    float fz3 = fz * fz * fz;
    return vec3(
        0.95047 * ((fx3 > 0.008856) ? fx3 : (fx - 16. / 116.) / 7.787),
        1.00000 * ((fy3 > 0.008856) ? fy3 : (fy - 16. / 116.) / 7.787),
        1.08883 * ((fz3 > 0.008856) ? fz3 : (fz - 16. / 116.) / 7.787)
    );
}

vec3 Cool_LinearRGB_from_XYZ(vec3 c)
{
    return c * mat3(3.2406, -1.5372, -0.4986, -0.9689, 1.8758, 0.0415, 0.0557, -0.2040, 1.0570);
}

vec3 Cool_sRGB_from_XYZ(vec3 c)
{
    return Cool_sRGB_from_LinearRGB(Cool_LinearRGB_from_XYZ(c));
}

vec3 Cool_sRGB_from_CIELAB(vec3 c)
{
    return Cool_sRGB_from_XYZ(Cool_XYZ_from_CIELAB(c));
}

vec3 Cool_LinearRGB_from_CIELAB(vec3 c)
{
    return Cool_LinearRGB_from_sRGB(Cool_sRGB_from_CIELAB(c));
}
// End of [Block2]

vec3 Cool_black_in_cielab = Cool_CIELAB_from_sRGB(vec3(0.));
vec3 Cool_white_in_cielab = Cool_CIELAB_from_sRGB(vec3(1.));