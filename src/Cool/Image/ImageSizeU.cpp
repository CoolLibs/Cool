#include "ImageSizeU.h"
#include <Cool/ImGui/ImGuiExtras.h>

namespace Cool::ImageSizeU {

auto imgui(img::Size& size) -> WH
{
    auto changed_side{WH::None};
    ImGui::Text("Resolution: ");
    ImGui::SameLine();
    {
        ImGui::PushItemWidth(ImGui::CalcTextSize("00000").x + ImGui::GetStyle().FramePadding.x); // Enough width for any 5-digit number.
        auto restore_item_width = sg::make_scope_guard([]() { ImGui::PopItemWidth(); });

        auto w = size.width();
        if (ImGuiExtras::input_uint("W", &w))
        {
            changed_side = WH::Width;
            size.set_width(w);
        }
        ImGui::SameLine();
        auto h = size.height();
        if (ImGuiExtras::input_uint("H", &h))
        {
            changed_side = WH::Height;
            size.set_height(h);
        }
    }
    return changed_side;
}

} // namespace Cool::ImageSizeU