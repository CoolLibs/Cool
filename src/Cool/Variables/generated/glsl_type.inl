/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "glsl_type" function there.
 * -----------------------------------------------------------------------------
 */

template<>
auto glsl_type<bool>() -> std::string
{
    return "bool";
}

template<>
auto glsl_type<int>() -> std::string
{
    return "int";
}

template<>
auto glsl_type<float>() -> std::string
{
    return "float";
}

template<>
auto glsl_type<Cool::Point2D>() -> std::string
{
    return "Point2D";
}

template<>
auto glsl_type<glm::vec2>() -> std::string
{
    return "vec2";
}

template<>
auto glsl_type<glm::vec3>() -> std::string
{
    return "vec3";
}

template<>
auto glsl_type<glm::vec4>() -> std::string
{
    return "vec4";
}

template<>
auto glsl_type<Cool::RgbColor>() -> std::string
{
    return "RgbColor";
}

template<>
auto glsl_type<Cool::Camera>() -> std::string
{
    return "Camera";
}

template<>
auto glsl_type<Cool::Angle>() -> std::string
{
    return "Angle";
}

template<>
auto glsl_type<Cool::Direction2D>() -> std::string
{
    return "Direction2D";
}

template<>
auto glsl_type<Cool::Hue>() -> std::string
{
    return "Hue";
}

template<>
auto glsl_type<Cool::Gradient>() -> std::string
{
    return "Gradient";
}
