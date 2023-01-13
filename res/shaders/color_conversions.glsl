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
struct Bounds {
        float a;
        float b;
};
  const float kappa = 903.29629629629629629630;
  const float epsilon = 0.00885645167903563082;

  const float ref_u = 0.19783000664283680764;
  const float ref_v = 0.46831999493879100370;

  const vec3 m[3] = {vec3(3.24096994190452134377, -1.53738317757009345794, -0.49861076029300328366), vec3(-0.96924363628087982613,  1.87596750150772066772,  0.04155505740717561247),  vec3(0.05563007969699360846, -0.20397695888897656435,  1.05697151424287856072)};

void get_bounds(float l,  Bounds bounds[6])
{
    float tl = l + 16.;
    float sub1 = (tl * tl * tl) / 1560896.;
    float sub2 = (sub1 >  epsilon ? sub1 : (l /  kappa));
    int t;

    for(int channel = 0; channel < 3; channel++) {
        float m1 =  m[channel].x;
        float m2 =  m[channel].y;
        float m3 =  m[channel].z;

        for (t = 0; t < 2; t++) {
            float top1 = (284517. * m1 - 94839. * m3) * sub2;
            float top2 = (838422. * m3 + 769860. * m2 + 731718. * m1) * l * sub2 -  769860. * t * l;
            float bottom = (632260. * m3 - 126452. * m2) * sub2 + 126452. * t;

            bounds[channel * 2 + t].a = top1 / bottom;
            bounds[channel * 2 + t].b = top2 / bottom;
        }
    }
}

float ray_length_until_intersect(float theta, Bounds line)
{
    return line.b / (sin(theta) - line.a * cos(theta));
}

float max_chroma_for_lh(float l, float h)
{
    float min_len = FLT_MAX;
    float hrad = h * 0.01745329251994329577; /* (2 * pi / 360) */
    Bounds bounds[6];
    get_bounds(l, bounds);
    for(int i = 0; i < 6; i++) {
        float len =  ray_length_until_intersect(hrad, bounds[i]);

        if(len >= 0  &&  len < min_len)
            min_len = len;
    }
    return min_len;
}

vec3 Cool_HSLuv_from_XYZ(vec3 c)
{
    //XYZ -> LUV
    float var_u = (4.0 * c.x) / (c.x + (15.0 * c.y) + (3.0 * c.z));
    float var_v = (9.0 * c.y) / (c.x + (15.0 * c.y) + (3.0 * c.z));
    float l = (c.y <=  epsilon) ? c.y *  kappa :  116.0 * pow(c.y,1./3.) - 16.0 ;
    float u = 13.0 * l * (var_u -  ref_u);
    float v = 13.0 * l * (var_v -  ref_v);

    c.x = l;
    if (l < 0.00000001){
        c.y = 0.;
        c.z = 0.;
    }

    else {
        c.y = u;
        c.z = v;
    }

    //LUV -> lch
    float h;
    float c2 = sqrt(u * u + v * v);

    /* Grays: disambiguate hue */
    if(c2 < 0.00000001) {
        h = 0;
    } else {
        h = atan(v, u) * 57.29577951308232087680;  /* (180 / pi) */
        if(h < 0.)
            h += 360.0;
    }

    //lch -> HSLuv
    float s;

    /* White and black: disambiguate saturation */
    if(l > 99.9999999 || l < 0.00000001) s = 0.;
    else s = c2 / max_chroma_for_lh(l, h)* 100.;

    /* Grays: disambiguate hue */
    if (c2 < 0.00000001)
        h = 0.;

    return  vec3(h,s,l);
}

vec3 Cool_XYZ_from_HSLuv( vec3 c)
{
    //HSLuv -> lch
    float h = c.x;
    float s = c.y;
    float l = c.z;
    float c2;

    /* White and black: disambiguate chroma */
    if(l > 99.9999999 || l < 0.00000001)
        c2 = 0.;
    else
        c2 =  max_chroma_for_lh(l,h);

    /* Grays: disambiguate hue */
    if (s < 0.00000001){
        h = 0.;
    }

    //lch -> LUV
    float hrad = h * 0.01745329251994329577;  /* (pi / 180.0) */
    float u = cos(hrad) * c2;
    float v = sin(hrad) * c2;

    //LUV -> XYZ
    if (l <= 0.00000001) return  vec3(0);

    float var_u = u / (13.0 * l) +  ref_u;
    float var_v = v / (13.0 * l) +  ref_v;
    float y = (l <= 8.0)? l /  kappa : ((l + 16.0) / 116.0)*((l + 16.0) / 116.0)*((l + 16.0) / 116.0);
    float x = -(9.0 * y * var_u) / ((var_u - 4.0) * var_v - var_u * var_v);
    float z = (9.0 * y - (15.0 * var_v * y) - (var_v * x)) / (3.0 * var_v);

    return  vec3(x,y,z);
}

//SRGB
vec3 Cool_HSLuv_from_sRGB( vec3 c)
{
    return Cool_HSLuv_from_XYZ(Cool_XYZ_from_sRGB(c));
}

vec3 Cool_sRGB_from_HSLuv( vec3 c){
    return Cool_sRGB_from_XYZ(Cool_XYZ_from_HSLuv(c));
}

//LinearRGB
vec3 Cool_HSLuv_from_LinearRGB( vec3 c){
    return Cool_HSLuv_from_XYZ(Cool_XYZ_from_LinearRGB(c));
}

vec3 Cool_LinearRGB_from_HSLuv( vec3 c){
    return Cool_LinearRGB_from_XYZ(Cool_XYZ_from_HSLuv(c));
}

//CIELAB
vec3 Cool_HSLuv_from_CIELAB( vec3 c){
    return Cool_HSLuv_from_XYZ(Cool_XYZ_from_CIELAB(c));
}

vec3 Cool_CIELAB_from_HSLuv( vec3 c){
    return Cool_CIELAB_from_XYZ(Cool_XYZ_from_HSLuv(c));
}
// End of [Block3]