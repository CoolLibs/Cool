#include "TestVariables.h"
#include <Cool/Variables/Variables.h>
#include <imgui/imgui.h>
#include <vector>
#include "Cool/Camera/Camera.h"
#include "Cool/Gpu/TextureDescriptor.h"
#include "Cool/StrongTypes/ColorAndAlpha.h"
#include "Cool/StrongTypes/MathExpression.h"
#include "Cool/StrongTypes/Point2D.h"

namespace Cool {

void test_variables()
{
    {
        static Cool::Variable<bool> var_bool{.name = "Bool", .value = {}};
        Cool::imgui(var_bool);
    }
    {
        static Cool::Variable<int> var_int{.name = "Int", .value = {}};
        Cool::imgui(var_int);
    }
    {
        static Cool::Variable<float> var_float{.name = "Float", .value = {}};
        Cool::imgui(var_float);
    }
    {
        static Cool::Variable<glm::vec2> vector2{.name = "Vector2", .value = {}};
        Cool::imgui(vector2);
    }
    {
        static Cool::Variable<glm::vec3> vector3{.name = "Vector3", .value = {}};
        Cool::imgui(vector3);
    }
    {
        static Cool::Variable<glm::vec4> vector4{.name = "Vector4", .value = {}};
        Cool::imgui(vector4);
    }
    {
        static Cool::Variable<Cool::Point2D> point2D{.name = "Point2D", .value = {}};
        Cool::imgui(point2D);
    }
    {
        static Cool::Variable<Cool::Angle> angle{.name = "Angle", .value = {}};
        Cool::imgui(angle);
    }
    {
        static Cool::Variable<Cool::Direction2D> direction2D{.name = "Direction 2D", .value = {}};
        Cool::imgui(direction2D);
    }
    {
        static Cool::Variable<Cool::Hue> hue{.name = "Hue", .value = {}};
        Cool::imgui(hue);
    }
    {
        static Cool::Variable<Cool::Color> color{.name = "Color", .value = Cool::Color::from_srgb(glm::vec3{0.f, 1.f, 0.5f})};
        Cool::imgui(color);
    }
    {
        static Cool::Variable<Cool::ColorAndAlpha> color{.name = "Color and Alpha", .value = Cool::ColorAndAlpha::from_srgb_straight_alpha(glm::vec4{1.f, 0.f, 0.5f, 1.f})};
        Cool::imgui(color);
    }
    {
        static Cool::Variable<Cool::ColorPalette> color_palette{
            .name  = "Color Palette",
            .value = Cool::ColorPalette{std::vector{
                Cool::Color::from_srgb(glm::vec3{0.f, 1.f, 0.75f}),
                Cool::Color::from_srgb(glm::vec3{1.f, 1.f, 0.5f}),
                Cool::Color::from_srgb(glm::vec3{1.f, 0.f, 0.5f}),
            }},
        };
        Cool::imgui(color_palette);
    }
    {
        static Cool::Variable<Cool::Gradient> gradient{.name = "Gradient", .value = {}};
        Cool::imgui(gradient);
    }
    {
        static Cool::Variable<Cool::Camera> camera{.name = "Camera 3D", .value = {}};
        Cool::imgui(camera);
    }
    {
        static Cool::Variable<Cool::Camera2D> camera{.name = "Camera 2D", .value = {}};
        Cool::imgui(camera);
    }
    {
        static Cool::Variable<Cool::MathExpression> math_expression{.name = "Math Expression", .value = {}};
        Cool::imgui(math_expression);
    }
    {
        static Cool::Variable<Cool::TextureDescriptor_FromFile> tex{.name = "Texture", .value = {}};
        Cool::imgui(tex);
    }
    {
        static Cool::Variable<Cool::TextureDescriptor_FromWebcam> webcam{.name = "Webcam", .value = {}};
        Cool::imgui(webcam);
    }
    {
        static Cool::Variable<Cool::MidiChannel> midi_channel{.name = "Midi", .value = {}};
        Cool::imgui(midi_channel);
    }
}

} // namespace Cool
