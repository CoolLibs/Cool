/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "always_requires_shader_code_generation" function there.
 * -----------------------------------------------------------------------------
 */

if constexpr (std::is_same_v<T, bool>)
    return false;

if constexpr (std::is_same_v<T, int>)
    return false;

if constexpr (std::is_same_v<T, float>)
    return false;

if constexpr (std::is_same_v<T, Cool::Point2D>)
    return false;

if constexpr (std::is_same_v<T, glm::vec2>)
    return false;

if constexpr (std::is_same_v<T, glm::vec3>)
    return false;

if constexpr (std::is_same_v<T, glm::vec4>)
    return false;

if constexpr (std::is_same_v<T, Cool::Color>)
    return false;

if constexpr (std::is_same_v<T, Cool::ColorAndAlpha>)
    return false;

if constexpr (std::is_same_v<T, Cool::Camera>)
    return false;

if constexpr (std::is_same_v<T, Cool::Camera2D>)
    return false;

if constexpr (std::is_same_v<T, Cool::Angle>)
    return false;

if constexpr (std::is_same_v<T, Cool::Direction2D>)
    return false;

if constexpr (std::is_same_v<T, Cool::Hue>)
    return false;

if constexpr (std::is_same_v<T, Cool::ColorPalette>)
    return true;

if constexpr (std::is_same_v<T, Cool::Gradient>)
    return false;

if constexpr (std::is_same_v<T, Cool::TextureDescriptor_Image>)
    return false;

if constexpr (std::is_same_v<T, Cool::TextureDescriptor_Webcam>)
    return false;

if constexpr (std::is_same_v<T, Cool::MathExpression>)
    return true;

if constexpr (std::is_same_v<T, glm::mat2>)
    return false;

if constexpr (std::is_same_v<T, glm::mat3>)
    return false;

if constexpr (std::is_same_v<T, glm::mat4>)
    return false;

if constexpr (std::is_same_v<T, Cool::MidiChannel>)
    return false;

if constexpr (std::is_same_v<T, Cool::OSCChannel>)
    return false;

if constexpr (std::is_same_v<T, Cool::TimeSpeed>)
    return false;

if constexpr (std::is_same_v<T, Cool::TextureDescriptor_Video>)
    return false;

if constexpr (std::is_same_v<T, Cool::Time>)
    return false;

if constexpr (std::is_same_v<T, Cool::TextureDescriptor_ImageSequence>)
    return false;
