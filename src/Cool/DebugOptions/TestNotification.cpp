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
        ImGui::Notify::add({.type = _type, .content = std::to_string(i++)});
    if (ImGui::Button("Error"))
        ImGui::Notify::add({.type = ImGui::Notify::ToastType::Error, .content = "Hello"});
    if (ImGui::Button("Warning"))
        ImGui::Notify::add({.type = ImGui::Notify::ToastType::Warning, .content = "Hello"});
    if (ImGui::Button("Success"))
        ImGui::Notify::add({.type = ImGui::Notify::ToastType::Success, .content = "Hello"});
    if (ImGui::Button("Info"))
        ImGui::Notify::add({.type = ImGui::Notify::ToastType::Info, .content = "Hello"});
    if (ImGui::Button("None"))
        ImGui::Notify::add({.type = ImGui::Notify::ToastType::None, .content = "Hello"});
}

} // namespace Cool
