#include "ImageSizeU.h"
#include <Cool/ImGui/ImGuiExtras.h>

namespace Cool::ImageSizeU {

bool imgui(img::Size& size)
{
    bool was_used = false;
    ImGui::Text("Resolution: ");
    ImGui::SameLine();
    {
        ImGui::PushItemWidth(50);
        auto restore_item_width = sg::make_scope_guard([]() { ImGui::PopItemWidth(); });

        auto w = size.width();
        if (ImGuiExtras::input_uint("W", &w))
        {
            was_used = true;
            size.set_width(w);
        }
        ImGui::SameLine();
        auto h = size.height();
        if (ImGuiExtras::input_uint("H", &h))
        {
            was_used = true;
            size.set_height(h);
        }
    }
    return was_used;
}

} // namespace Cool::ImageSizeU