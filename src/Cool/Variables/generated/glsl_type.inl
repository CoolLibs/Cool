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
    return "vec2";
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
auto glsl_type<Cool::Color>() -> std::string
{
    return "vec3";
}

template<>
auto glsl_type<Cool::ColorAndAlpha>() -> std::string
{
    return "vec4";
}

template<>
auto glsl_type<Cool::Camera>() -> std::string
{
    return "mat4";
}

template<>
auto glsl_type<Cool::Camera2D>() -> std::string
{
    return "mat3";
}

template<>
auto glsl_type<Cool::Angle>() -> std::string
{
    return "float";
}

template<>
auto glsl_type<Cool::Direction2D>() -> std::string
{
    return "vec2";
}

template<>
auto glsl_type<Cool::Hue>() -> std::string
{
    return "float";
}

template<>
auto glsl_type<Cool::ColorPalette>() -> std::string
{
    return "NO TYPE THIS IS A FUNCTION";
}

template<>
auto glsl_type<Cool::Gradient>() -> std::string
{
    return "NO TYPE THIS IS A FUNCTION";
}

template<>
auto glsl_type<Cool::TextureDescriptor_FromFile>() -> std::string
{
    return "Cool_Texture";
}

template<>
auto glsl_type<Cool::TextureDescriptor_FromWebcam>() -> std::string
{
    return "Cool_Texture";
}

template<>
auto glsl_type<Cool::MathExpression>() -> std::string
{
    return "NO TYPE THIS IS A FUNCTION";
}

template<>
auto glsl_type<glm::mat2>() -> std::string
{
    return "mat2";
}

template<>
auto glsl_type<glm::mat3>() -> std::string
{
    return "mat3";
}

template<>
auto glsl_type<glm::mat4>() -> std::string
{
    return "mat4";
}

template<>
auto glsl_type<Cool::MidiChannel>() -> std::string
{
    return "float";
}

template<>
auto glsl_type<Cool::OSCChannel>() -> std::string
{
    return "float";
}

template<>
auto glsl_type<Cool::TimeSpeed>() -> std::string
{
    return "float";
}

template<>
auto glsl_type<Cool::TextureDescriptor_VideoFile>() -> std::string
{
    return "Cool_Texture";
}
