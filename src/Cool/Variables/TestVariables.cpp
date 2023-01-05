#if DEBUG

#include "TestVariables.h"
#include <Cool/Variables/Variables.h>
#include <imgui/imgui.h>
#include <vector>

namespace Cool {

void test_variables()
{
    static Cool::Variable<Cool::Angle> angle{
        "Angle",
        0.278_turn};
    static Cool::Variable<Cool::Hue> hue{
        "Hue",
        Cool::Hue{0.5f}};
    static Cool::Variable<Cool::Color> color{
        "Color",
        Cool::Color::from_srgb(glm::vec3{0.f, 1.f, 0.5f})};
    static Cool::Variable<Cool::Direction2D> direction{
        "Direction",
        Cool::Direction2D{glm::vec2{0.f, 10.f}}};
    static Cool::Variable<glm::vec4> vector4{
        "Vector4",
        glm::vec4{0.f, 1.f, 0.5f, 8.f}};
    static Cool::Variable<glm::vec3> vector3{
        "Vector3",
        glm::vec3{1.f, 1.5f, 8.f}};
    static Cool::Variable<Cool::ColorPalette> color_palette{
        "Color Palette",
        Cool::ColorPalette{
            std::vector{
                Cool::Color::from_srgb(glm::vec3{0.f, 1.f, 0.75f}),
                Cool::Color::from_srgb(glm::vec3{1.f, 1.f, 0.5f}),
                Cool::Color::from_srgb(glm::vec3{1.f, 0.f, 0.5f}),
            }}};
    static Cool::Variable<Cool::Gradient> gradient{
        "Gradient",
        Cool::Gradient{
            ImGG::GradientWidget{}}};

    Cool::imgui(angle);
    Cool::imgui(color);
    Cool::imgui(hue);
    Cool::imgui(direction);
    Cool::imgui(vector3);
    Cool::imgui(vector4);
    Cool::imgui(color_palette);
    Cool::imgui(gradient);
}

} // namespace Cool

#endif