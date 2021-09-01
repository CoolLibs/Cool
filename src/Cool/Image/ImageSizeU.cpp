#include "ImageSizeU.h"
#include <Cool/ImGuiExtras/ImGuiExtras.h>
#include <scope_guard/scope_guard.hpp>

namespace Cool::ImageSizeU {

bool imgui(ImageSize& size)
{
    bool was_used = false;
    ImGui::Text("Resolution : ");
    ImGui::SameLine();
    {
        ImGui::PushItemWidth(50);
        auto restore_item_width = sg::make_scope_guard([]() { ImGui::PopItemWidth(); });

        auto w = size.width();
        if (ImGuiExtras::input_uint("W", &w)) {
            was_used = true;
            if (w >= 1) {
                size.set_width(w);
            }
        }
        ImGui::SameLine();
        auto h = size.height();
        if (ImGuiExtras::input_uint("H", &h)) {
            was_used = true;
            if (h >= 1) {
                size.set_height(h);
            }
        }
    }
    return was_used;
}

} // namespace Cool::ImageSizeU