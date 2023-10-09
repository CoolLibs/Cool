/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_colors.py" and edit the "define_color_getters" function there.
 * -----------------------------------------------------------------------------
 */

auto Color::as_CIELAB() const -> glm::vec3
{
    return CIELAB_from_sRGB(_srgb);
}

auto Color::as_Oklab() const -> glm::vec3
{
    return Oklab_from_sRGB(_srgb);
}

auto Color::as_LinearRGB() const -> glm::vec3
{
    return LinearRGB_from_sRGB(_srgb);
}

auto Color::as_sRGB() const -> glm::vec3
{
    return (_srgb);
}

auto Color::as_HSLuv() const -> glm::vec3
{
    return HSLuv_from_sRGB(_srgb);
}
