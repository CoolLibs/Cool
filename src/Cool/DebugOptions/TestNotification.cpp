#include "TestNotification.h"
#include <imgui.h>
#include <ImGuiNotify/ImGuiNotify.hpp>

namespace Cool {

TestNotification::TestNotification()
{
}

void TestNotification::imgui()
{
    if (ImGui::Button("Send "))
        ImGuiNotify::send({.type = _type, .content = std::to_string(i++)});
    if (ImGui::Button("Error"))
        ImGuiNotify::send({.type = ImGuiNotify::Type::Error, .content = "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHello"});
    if (ImGui::Button("Warning"))
        ImGuiNotify::send({.type = ImGuiNotify::Type::Warning, .title = "Warning", .content = "Hello"});
    if (ImGui::Button("Success"))
        ImGuiNotify::send({.type = ImGuiNotify::Type::Success, .content = "Hello"});
    if (ImGui::Button("Info"))
        ImGuiNotify::send({.type = ImGuiNotify::Type::Info, .title = "Hello"});
}

} // namespace Cool
