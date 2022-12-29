/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_tfs.py" and edit the "evaluate_function_template" function there.
 * -----------------------------------------------------------------------------
 */
#define COOL_TFS_EVALUATE_FUNCTION_TEMPLATE(function_template, type_as_string, out_type, arguments)                                                                                                                                                                                       \
    ([&]() -> out_type {                                                                                                                                                                                                                                                                  \
        if ((type_as_string) == "bool")                                                                                                                                                                                                                                                   \
            return function_template<bool> arguments;                                                                                                                                                                                                                                     \
        else if ((type_as_string) == "int")                                                                                                                                                                                                                                               \
            return function_template<int> arguments;                                                                                                                                                                                                                                      \
        else if ((type_as_string) == "float")                                                                                                                                                                                                                                             \
            return function_template<float> arguments;                                                                                                                                                                                                                                    \
        else if ((type_as_string) == "UV")                                                                                                                                                                                                                                                \
            return function_template<Cool::Point2D> arguments;                                                                                                                                                                                                                            \
        else if ((type_as_string) == "Point2D")                                                                                                                                                                                                                                           \
            return function_template<Cool::Point2D> arguments;                                                                                                                                                                                                                            \
        else if ((type_as_string) == "vec2")                                                                                                                                                                                                                                              \
            return function_template<glm::vec2> arguments;                                                                                                                                                                                                                                \
        else if ((type_as_string) == "vec3")                                                                                                                                                                                                                                              \
            return function_template<glm::vec3> arguments;                                                                                                                                                                                                                                \
        else if ((type_as_string) == "vec4")                                                                                                                                                                                                                                              \
            return function_template<glm::vec4> arguments;                                                                                                                                                                                                                                \
        else if ((type_as_string) == "sRGB")                                                                                                                                                                                                                                              \
            return function_template<Cool::Color> arguments;                                                                                                                                                                                                                              \
        else if ((type_as_string) == "sRGB_StraightA")                                                                                                                                                                                                                                    \
            return function_template<Cool::ColorAndAlpha> arguments;                                                                                                                                                                                                                      \
        else if ((type_as_string) == "Camera")                                                                                                                                                                                                                                            \
            return function_template<Cool::Camera> arguments;                                                                                                                                                                                                                             \
        else if ((type_as_string) == "Angle")                                                                                                                                                                                                                                             \
            return function_template<Cool::Angle> arguments;                                                                                                                                                                                                                              \
        else if ((type_as_string) == "Direction2D")                                                                                                                                                                                                                                       \
            return function_template<Cool::Direction2D> arguments;                                                                                                                                                                                                                        \
        else if ((type_as_string) == "Hue")                                                                                                                                                                                                                                               \
            return function_template<Cool::Hue> arguments;                                                                                                                                                                                                                                \
        else if ((type_as_string) == "ColorPalette")                                                                                                                                                                                                                                      \
            return function_template<Cool::ColorPalette> arguments;                                                                                                                                                                                                                       \
        else if ((type_as_string) == "Gradient")                                                                                                                                                                                                                                          \
            return function_template<Cool::Gradient> arguments;                                                                                                                                                                                                                           \
        else                                                                                                                                                                                                                                                                              \
        {                                                                                                                                                                                                                                                                                 \
            throw std::runtime_error{"Unknown type \"" + std::string{type_as_string} + "\". The possible types are:\n- bool\n- int\n- float\n- UV (aka Point2D)\n- vec2\n- vec3\n- vec4\n- sRGB\n- sRGB_StraightA\n- Camera\n- Angle\n- Direction2D\n- Hue\n- ColorPalette\n- Gradient"}; \
        }                                                                                                                                                                                                                                                                                 \
    })()
