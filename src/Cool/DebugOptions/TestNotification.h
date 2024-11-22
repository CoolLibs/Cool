#pragma once
#include "ImGuiNotify/ImGuiNotify.hpp"

namespace Cool {

class TestNotification {
public:
    TestNotification();

    void imgui();

private:
    std::string              _message{"Hello"};
    ImGui::Notify::ToastType _type{ImGui::Notify::ToastType::Success};
    int                      i{0};
};

} // namespace Cool