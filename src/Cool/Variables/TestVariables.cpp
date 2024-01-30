#include "TestVariables.h"
#include <Cool/Variables/Variables.h>
#include <imgui/imgui.h>
#include <vector>
#include "Cool/Camera/Camera.h"
#include "Cool/Gpu/TextureDescriptor.h"
#include "Cool/OSC/OSCChannel.h"
#include "Cool/StrongTypes/ColorAndAlpha.h"
#include "Cool/StrongTypes/MathExpression.h"
#include "Cool/StrongTypes/Point2D.h"

namespace Cool {

void test_variables()
{
    {
        static Cool::Variable<bool> var_bool{{"Bool"}};
        Cool::imgui(var_bool);
    }
    {
        static Cool::Variable<int> var_int{{"Int"}};
        Cool::imgui(var_int);
    }
    {
        static Cool::Variable<float> var_float{{"Float"}};
        Cool::imgui(var_float);
    }
    {
        static Cool::Variable<glm::vec2> vector2{{"Vector2"}};
        Cool::imgui(vector2);
    }
    {
        static Cool::Variable<glm::vec3> vector3{{"Vector3"}};
        Cool::imgui(vector3);
    }
    {
        static Cool::Variable<glm::vec4> vector4{{"Vector4"}};
        Cool::imgui(vector4);
    }
    {
        static Cool::Variable<Cool::Point2D> point2D{{"Point2D"}};
        Cool::imgui(point2D);
    }
    {
        static Cool::Variable<Cool::Angle> angle{{"Angle"}};
        Cool::imgui(angle);
    }
    {
        static Cool::Variable<Cool::Direction2D> direction2D{{"Direction 2D"}};
        Cool::imgui(direction2D);
    }
    {
        static Cool::Variable<Cool::Hue> hue{{"Hue"}};
        Cool::imgui(hue);
    }
    {
        static Cool::Variable<Cool::Color> color{{"Color", Cool::Color::from_srgb(glm::vec3{0.f, 1.f, 0.5f})}};
        Cool::imgui(color);
    }
    {
        static Cool::Variable<Cool::ColorAndAlpha> color{{"Color and Alpha", Cool::ColorAndAlpha::from_srgb_straight_alpha(glm::vec4{1.f, 0.f, 0.5f, 1.f})}};
        Cool::imgui(color);
    }
    {
        static Cool::Variable<Cool::ColorPalette> color_palette{{"Color Palette", Cool::ColorPalette{std::vector{Cool::Color::from_srgb(glm::vec3{0.f, 1.f, 0.75f}), Cool::Color::from_srgb(glm::vec3{1.f, 1.f, 0.5f}), Cool::Color::from_srgb(glm::vec3{1.f, 0.f, 0.5f})}}}};
        Cool::imgui(color_palette);
    }
    {
        static Cool::Variable<Cool::Gradient> gradient{{"Gradient"}};
        Cool::imgui(gradient);
    }
    {
        static Cool::Variable<Cool::Camera> camera{{"Camera 3D"}};
        Cool::imgui(camera);
    }
    {
        static Cool::Variable<Cool::Camera2D> camera{{"Camera 2D"}};
        Cool::imgui(camera);
    }
    {
        static Cool::Variable<Cool::MathExpression> math_expression{{"Math Expression"}};
        Cool::imgui(math_expression);
    }
    {
        static Cool::Variable<Cool::TextureDescriptor_FromFile> tex{{"Texture"}};
        Cool::imgui(tex);
    }
    {
        static Cool::Variable<Cool::TextureDescriptor_FromWebcam> webcam{{"Webcam"}};
        Cool::imgui(webcam);
    }
    {
        static Cool::Variable<Cool::MidiChannel> midi_channel{{"Midi"}};
        Cool::imgui(midi_channel);
    }
    {
        static Cool::Variable<Cool::OSCChannel> osc_channel{{"OSC"}};
        Cool::imgui(osc_channel);
    }
}

} // namespace Cool
