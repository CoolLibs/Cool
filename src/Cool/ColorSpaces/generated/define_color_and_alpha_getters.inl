/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_colors.py" and edit the "define_color_and_alpha_getters" function there.
 * -----------------------------------------------------------------------------
 */

auto ColorAndAlpha::as_CIELAB_StraightA() const -> glm::vec4
{
    return {
        CIELAB_from_sRGB(glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }),
        _srgb_straight.a,
    };
}
auto ColorAndAlpha::as_CIELAB_PremultipliedA() const -> glm::vec4
{
    return {
        CIELAB_from_sRGB(glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }) * _srgb_straight.a,
        _srgb_straight.a,
    };
}
auto ColorAndAlpha::as_Oklab_StraightA() const -> glm::vec4
{
    return {
        Oklab_from_sRGB(glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }),
        _srgb_straight.a,
    };
}
auto ColorAndAlpha::as_Oklab_PremultipliedA() const -> glm::vec4
{
    return {
        Oklab_from_sRGB(glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }) * _srgb_straight.a,
        _srgb_straight.a,
    };
}
auto ColorAndAlpha::as_LinearRGB_StraightA() const -> glm::vec4
{
    return {
        LinearRGB_from_sRGB(glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }),
        _srgb_straight.a,
    };
}
auto ColorAndAlpha::as_LinearRGB_PremultipliedA() const -> glm::vec4
{
    return {
        LinearRGB_from_sRGB(glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }) * _srgb_straight.a,
        _srgb_straight.a,
    };
}
auto ColorAndAlpha::as_sRGB_StraightA() const -> glm::vec4
{
    return {
        (glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }),
        _srgb_straight.a,
    };
}
auto ColorAndAlpha::as_sRGB_PremultipliedA() const -> glm::vec4
{
    return {
        (glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }) * _srgb_straight.a,
        _srgb_straight.a,
    };
}
auto ColorAndAlpha::as_HSLuv_StraightA() const -> glm::vec4
{
    return {
        HSLuv_from_sRGB(glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }),
        _srgb_straight.a,
    };
}
auto ColorAndAlpha::as_HSLuv_PremultipliedA() const -> glm::vec4
{
    return {
        HSLuv_from_sRGB(glm::vec3{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }) * _srgb_straight.a,
        _srgb_straight.a,
    };
}