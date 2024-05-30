#include "TestVariables.h"
#include <Cool/Variables/Variables.h>

namespace Cool {

void test_variables()
{
    {
        static auto var = Cool::Variable<bool>{{"Bool"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<int>{{"Int"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<float>{{"Float"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<glm::vec2>{{"Vector2"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<glm::vec3>{{"Vector3"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<glm::vec4>{{"Vector4"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::Point2D>{{"Point2D"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::Angle>{{"Angle"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::Direction2D>{{"Direction 2D"}};
        var.imgui();
    }
    {
        static auto hue = Cool::Variable<Cool::Hue>{{"Hue"}};
        hue.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::Color>{{"Color", Cool::Color::from_srgb(glm::vec3{0.f, 1.f, 0.5f})}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::ColorAndAlpha>{{"Color and Alpha", Cool::ColorAndAlpha::from_srgb_straight_alpha(glm::vec4{1.f, 0.f, 0.5f, 1.f})}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::ColorPalette>{{"Color Palette", Cool::ColorPalette{std::vector{Cool::Color::from_srgb(glm::vec3{0.f, 1.f, 0.75f}), Cool::Color::from_srgb(glm::vec3{1.f, 1.f, 0.5f}), Cool::Color::from_srgb(glm::vec3{1.f, 0.f, 0.5f})}}}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::Gradient>{{"Gradient"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::Camera>{{"Camera 3D"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::Camera2D>{{"Camera 2D"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::MathExpression>{{"Math Expression"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::TextureDescriptor_Image>{{"Texture"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::TextureDescriptor_Webcam>{{"Webcam"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::MidiChannel>{{"Midi"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::OSCChannel>{{"OSC"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::Time>{{"Time"}};
        var.imgui();
    }
    {
        static auto var = Cool::Variable<Cool::TimeSpeed>{{"Time Speed"}};
        var.imgui();
    }
}

} // namespace Cool
