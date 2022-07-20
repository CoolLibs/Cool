/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_variables.py" and edit the "AnyInputRefToConst" function there.
 * -----------------------------------------------------------------------------
 */

using AnyInputRefToConst = std::variant<
    std::reference_wrapper<const Input<bool>>,
    std::reference_wrapper<const Input<int>>,
    std::reference_wrapper<const Input<float>>,
    std::reference_wrapper<const Input<glm::vec2>>,
    std::reference_wrapper<const Input<glm::vec3>>,
    std::reference_wrapper<const Input<glm::vec4>>,
    std::reference_wrapper<const Input<Cool::RgbColor>>,
    std::reference_wrapper<const Input<Cool::Camera>>,
    std::reference_wrapper<const Input<Cool::Angle>>,
    std::reference_wrapper<const Input<Cool::Direction2D>>,
    std::reference_wrapper<const Input<Cool::Hue>>>;