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
    return vec3(
        (1.16 * v.y) - 0.16,
        5. * (v.x - v.y),
        2. * (v.y - v.z)
    );
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
    float fy = (c.x + 0.16) / 1.16;
    float fx = c.y / 5. + fy;
    float fz = fy - c.z / 2.;

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

// Start of [Block3]
// https://github.com/williammalo/hsluv-glsl
/*
HSLUV-GLSL v4.2
HSLUV is a human-friendly alternative to HSL. ( http://www.hsluv.org )
GLSL port by William Malo ( https://github.com/williammalo )
Put this code in your fragment shader.
*/

vec3 CoolHsluv_intersectLineLine(vec3 line1x, vec3 line1y, vec3 line2x, vec3 line2y)
{
    return (line1y - line2y) / (line2x - line1x);
}

vec3 CoolHsluv_distanceFromPole(vec3 pointx, vec3 pointy)
{
    return sqrt(pointx * pointx + pointy * pointy);
}

vec3 CoolHsluv_lengthOfRayUntilIntersect(float theta, vec3 x, vec3 y)
{
    vec3 len = y / (sin(theta) - x * cos(theta));
    if (len.r < 0.0) {len.r=1000.0;}
    if (len.g < 0.0) {len.g=1000.0;}
    if (len.b < 0.0) {len.b=1000.0;}
    return len;
}

float CoolHsluv_maxSafeChromaForL(float L)
{
    mat3 m2 = mat3(
        3.2409699419045214, -0.96924363628087983, 0.055630079696993609,
        -1.5373831775700935, 1.8759675015077207, -0.20397695888897657,
        -0.49861076029300328, 0.041555057407175613, 1.0569715142428786
    );
    float sub0 = L + 16.0;
    float sub1 = sub0 * sub0 * sub0 * .000000641;
    float sub2 = sub1 > 0.0088564516790356308 ? sub1 : L / 903.2962962962963;

    vec3 top1   = (284517.0 * m2[0] - 94839.0 * m2[2]) * sub2;
    vec3 bottom = (632260.0 * m2[2] - 126452.0 * m2[1]) * sub2;
    vec3 top2   = (838422.0 * m2[2] + 769860.0 * m2[1] + 731718.0 * m2[0]) * L * sub2;

    vec3 bounds0x = top1 / bottom;
    vec3 bounds0y = top2 / bottom;

    vec3 bounds1x = top1 / (bottom + 126452.0);
    vec3 bounds1y = (top2 - 769860.0 * L) / (bottom + 126452.0);

    vec3 xs0 = CoolHsluv_intersectLineLine(bounds0x, bounds0y, -1.0 / bounds0x, vec3(0.0));
    vec3 xs1 = CoolHsluv_intersectLineLine(bounds1x, bounds1y, -1.0 / bounds1x, vec3(0.0));

    vec3 lengths0 = CoolHsluv_distanceFromPole(xs0, bounds0y + xs0 * bounds0x);
    vec3 lengths1 = CoolHsluv_distanceFromPole(xs1, bounds1y + xs1 * bounds1x);

    return  min(lengths0.r,
            min(lengths1.r,
            min(lengths0.g,
            min(lengths1.g,
            min(lengths0.b,
                lengths1.b)))));
}

float CoolHsluv_maxChromaForLH(float L, float H)
{
    float hrad = radians(H);

    mat3 m2 = mat3(
        3.2409699419045214, -0.96924363628087983, 0.055630079696993609,
        -1.5373831775700935, 1.8759675015077207, -0.20397695888897657,
        -0.49861076029300328, 0.041555057407175613, 1.0569715142428786
    );
    float sub1 = pow(L + 16.0, 3.0) / 1560896.0;
    float sub2 = sub1 > 0.0088564516790356308 ? sub1 : L / 903.2962962962963;

    vec3 top1   = (284517.0 * m2[0] - 94839.0 * m2[2]) * sub2;
    vec3 bottom = (632260.0 * m2[2] - 126452.0 * m2[1]) * sub2;
    vec3 top2   = (838422.0 * m2[2] + 769860.0 * m2[1] + 731718.0 * m2[0]) * L * sub2;

    vec3 bound0x = top1 / bottom;
    vec3 bound0y = top2 / bottom;

    vec3 bound1x = top1 / (bottom + 126452.0);
    vec3 bound1y = (top2 - 769860.0 * L) / (bottom + 126452.0);

    vec3 lengths0 = CoolHsluv_lengthOfRayUntilIntersect(hrad, bound0x, bound0y);
    vec3 lengths1 = CoolHsluv_lengthOfRayUntilIntersect(hrad, bound1x, bound1y);

    return  min(lengths0.r,
            min(lengths1.r,
            min(lengths0.g,
            min(lengths1.g,
            min(lengths0.b,
                lengths1.b)))));
}

float CoolHsluv_yToL(float Y)
{
    return Y <= 0.0088564516790356308 ? Y * 903.2962962962963 : 116.0 * pow(Y, 1.0 / 3.0) - 16.0;
}

float CoolHsluv_lToY(float L)
{
    return L <= 8.0 ? L / 903.2962962962963 : pow((L + 16.0) / 116.0, 3.0);
}

vec3 CoolHsluv_xyzToLuv(vec3 tuple)
{
    float X = tuple.x;
    float Y = tuple.y;
    float Z = tuple.z;

    float L = CoolHsluv_yToL(Y);

    float div = 1. / dot(tuple, vec3(1, 15, 3));

    return vec3(
               1.,
               (52. * (X * div) - 2.57179),
               (117. * (Y * div) - 6.08816)
           )
           * L;
}

vec3 CoolHsluv_luvToXyz(vec3 tuple)
{
    float L = tuple.x;

    float U = tuple.y / (13.0 * L) + 0.19783000664283681;
    float V = tuple.z / (13.0 * L) + 0.468319994938791;

    float Y = CoolHsluv_lToY(L);
    float X = 2.25 * U * Y / V;
    float Z = (3. / V - 5.) * Y - (X / 3.);

    return vec3(X, Y, Z);
}

vec3 CoolHsluv_luvToLch(vec3 tuple)
{
    float L = tuple.x;
    float U = tuple.y;
    float V = tuple.z;

    float C = length(tuple.yz);
    float H = degrees(atan(V, U));
    if (H < 0.0)
    {
        H = 360.0 + H;
    }

    return vec3(L, C, H);
}

vec3 CoolHsluv_lchToLuv(vec3 tuple)
{
    float hrad = radians(tuple.b);
    return vec3(
        tuple.r,
        cos(hrad) * tuple.g,
        sin(hrad) * tuple.g
    );
}

vec3 CoolHsluv_hsluvToLch(vec3 tuple)
{
    tuple.g *= CoolHsluv_maxChromaForLH(tuple.b, tuple.r) * .01;
    return tuple.bgr;
}

vec3 CoolHsluv_lchToHsluv(vec3 tuple)
{
    tuple.g /= CoolHsluv_maxChromaForLH(tuple.r, tuple.b) * .01;
    return tuple.bgr;
}

vec3 CoolHsluv_lchToXyz(vec3 tuple)
{
    return CoolHsluv_luvToXyz(CoolHsluv_lchToLuv(tuple));
}

vec3 CoolHsluv_xyzToLch(vec3 tuple)
{
    return CoolHsluv_luvToLch(CoolHsluv_xyzToLuv(tuple));
}

vec3 Cool_XYZ_from_HSLuv(vec3 tuple)
{
    return CoolHsluv_lchToXyz(CoolHsluv_hsluvToLch(saturate(tuple) * vec3(360., 100., 100.)));
}

vec3 Cool_HSLuv_from_XYZ(vec3 tuple)
{
    return CoolHsluv_lchToHsluv(CoolHsluv_xyzToLch(saturate(tuple))) / vec3(360., 100., 100.);
}

// SRGB
vec3 Cool_HSLuv_from_sRGB(vec3 c)
{
    return Cool_HSLuv_from_XYZ(Cool_XYZ_from_sRGB(c));
}

vec3 Cool_sRGB_from_HSLuv(vec3 c)
{
    return Cool_sRGB_from_XYZ(Cool_XYZ_from_HSLuv(c));
}

// LinearRGB
vec3 Cool_HSLuv_from_LinearRGB(vec3 c)
{
    return Cool_HSLuv_from_XYZ(Cool_XYZ_from_LinearRGB(c));
}

vec3 Cool_LinearRGB_from_HSLuv(vec3 c)
{
    return Cool_LinearRGB_from_XYZ(Cool_XYZ_from_HSLuv(c));
}

// CIELAB
vec3 Cool_HSLuv_from_CIELAB(vec3 c)
{
    return Cool_HSLuv_from_XYZ(Cool_XYZ_from_CIELAB(c));
}

vec3 Cool_CIELAB_from_HSLuv(vec3 c)
{
    return Cool_CIELAB_from_XYZ(Cool_XYZ_from_HSLuv(c));
}
// End of [Block3]

// Start of [Block4]
vec4 Cool_apply_straight_alpha_to_color(vec3 color, float alpha)
{
    return vec4(color, alpha);
}

vec4 Cool_apply_premultiplied_alpha_to_color(vec3 color, float alpha)
{
    return vec4(color, 1.) * alpha;
}

vec2 Cool_apply_straight_alpha_to_greyscale(float greyscale, float alpha)
{
    return vec2(greyscale, alpha);
}

vec2 Cool_apply_premultiplied_alpha_to_greyscale(float greyscale, float alpha)
{
    return vec2(greyscale, 1.) * alpha;
}

vec4 Cool_apply_straight_alpha_to_color_and_alpha(vec4 color, float alpha)
{
    return vec4(color.rgb, color.a * alpha);
}

vec4 Cool_apply_premultiplied_alpha_to_color_and_alpha(vec4 color, float alpha)
{
    return color * alpha;
}

vec2 Cool_apply_straight_alpha_to_greyscale_and_alpha(vec2 greyscale, float alpha)
{
    return vec2(greyscale.x, greyscale.y * alpha);
}

vec2 Cool_apply_premultiplied_alpha_to_greyscale_and_alpha(vec2 greyscale, float alpha)
{
    return greyscale * alpha;
}
// End of [Block4]

// Start of [Block5]
vec3 Cool_CIELAB_from_Float(float x)
{
    return vec3(saturate(x), 0., 0.);
}
float Cool_Float_from_CIELAB(vec3 lab)
{
    return lab.x;
}

vec3 Cool_sRGB_from_Float(float x)
{
    return Cool_sRGB_from_CIELAB(Cool_CIELAB_from_Float(x));
}
float Cool_Float_from_sRGB(vec3 color)
{
    return Cool_Float_from_CIELAB(Cool_CIELAB_from_sRGB(color));
}

vec3 Cool_LinearRGB_from_Float(float x)
{
    return Cool_LinearRGB_from_CIELAB(Cool_CIELAB_from_Float(x));
}
float Cool_Float_from_LinearRGB(vec3 color)
{
    return Cool_Float_from_CIELAB(Cool_CIELAB_from_LinearRGB(color));
}

vec3 Cool_HSLuv_from_Float(float x)
{
    return Cool_HSLuv_from_CIELAB(Cool_CIELAB_from_Float(x));
}
float Cool_Float_from_HSLuv(vec3 color)
{
    return Cool_Float_from_CIELAB(Cool_CIELAB_from_HSLuv(color));
}

// End of [Block5]