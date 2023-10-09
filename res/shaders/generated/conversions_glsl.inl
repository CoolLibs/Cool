/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_colors.py" and edit the "conversions_glsl" function there.
 * -----------------------------------------------------------------------------
 */
vec3 Cool_Oklab_from_LinearRGB(vec3 c)
{
    return Cool_Oklab_from_XYZ(Cool_XYZ_from_LinearRGB(c));
}
vec3 Cool_Oklab_from_sRGB(vec3 c)
{
    return Cool_Oklab_from_XYZ(Cool_XYZ_from_sRGB(c));
}
vec3 Cool_Oklab_from_HSLuv(vec3 c)
{
    return Cool_Oklab_from_XYZ(Cool_XYZ_from_HSLuv(c));
}
vec3 Cool_LinearRGB_from_Oklab(vec3 c)
{
    return Cool_LinearRGB_from_XYZ(Cool_XYZ_from_Oklab(c));
}
vec3 Cool_LinearRGB_from_HSLuv(vec3 c)
{
    return Cool_LinearRGB_from_XYZ(Cool_XYZ_from_HSLuv(c));
}
vec3 Cool_sRGB_from_Oklab(vec3 c)
{
    return Cool_sRGB_from_XYZ(Cool_XYZ_from_Oklab(c));
}
vec3 Cool_sRGB_from_HSLuv(vec3 c)
{
    return Cool_sRGB_from_XYZ(Cool_XYZ_from_HSLuv(c));
}
vec3 Cool_HSLuv_from_Oklab(vec3 c)
{
    return Cool_HSLuv_from_XYZ(Cool_XYZ_from_Oklab(c));
}
vec3 Cool_HSLuv_from_LinearRGB(vec3 c)
{
    return Cool_HSLuv_from_XYZ(Cool_XYZ_from_LinearRGB(c));
}
vec3 Cool_HSLuv_from_sRGB(vec3 c)
{
    return Cool_HSLuv_from_XYZ(Cool_XYZ_from_sRGB(c));
}