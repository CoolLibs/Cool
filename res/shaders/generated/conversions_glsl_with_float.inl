/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_colors.py" and edit the "conversions_glsl_with_float" function there.
 * -----------------------------------------------------------------------------
 */
vec3 Cool_Oklab_from_Float(float x)
{
    return Cool_Oklab_from_Okhsl(Cool_Okhsl_from_Float(x));
}
float Cool_Float_from_Oklab(vec3 color)
{
    return Cool_Float_from_Okhsl(Cool_Okhsl_from_Oklab(color));
}
vec3 Cool_LinearRGB_from_Float(float x)
{
    return Cool_LinearRGB_from_Okhsl(Cool_Okhsl_from_Float(x));
}
float Cool_Float_from_LinearRGB(vec3 color)
{
    return Cool_Float_from_Okhsl(Cool_Okhsl_from_LinearRGB(color));
}
vec3 Cool_sRGB_from_Float(float x)
{
    return Cool_sRGB_from_Okhsl(Cool_Okhsl_from_Float(x));
}
float Cool_Float_from_sRGB(vec3 color)
{
    return Cool_Float_from_Okhsl(Cool_Okhsl_from_sRGB(color));
}