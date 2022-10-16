#include "AspectRatio.h"
#include <smart/smart.hpp>
#include <stringify/stringify.hpp>

namespace Cool {

static float make_valid_ratio(float ratio)
{
    return smart::keep_above(0.001f, ratio);
}

AspectRatio::AspectRatio(float aspect_ratio)
    : _ratio{make_valid_ratio(aspect_ratio)}
{}

void AspectRatio::set(float aspect_ratio)
{
    _ratio = make_valid_ratio(aspect_ratio);
}

auto AspectRatio::imgui(float width) -> bool
{
    bool b = false;

    static constexpr std::array ratios = {
        std::make_pair("     16 / 9     ", 16.f / 9.f),
        std::make_pair("      3 / 2     ", 3.f / 2.f),
        std::make_pair("      4 / 3     ", 4.f / 3.f),
        std::make_pair("      1 / 1     ", 1.f),
        std::make_pair("      9 / 16     ", 9.f / 16.f),
        std::make_pair("      2 / 3     ", 2.f / 3.f),
        std::make_pair("      3 / 4     ", 3.f / 4.f),
    };

    if (ImGui::BeginCombo("##aspect_ratio_dropdown", "", ImGuiComboFlags_NoPreview))
    {
        for (auto const& ratio : ratios)
        {
            if (ImGui::Selectable(ratio.first, false))
            {
                _ratio = ratio.second;
                b      = true;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine(0.f, 0.f);
    if (width != 0.f)
        ImGui::SetNextItemWidth(width);
    if (ImGui::SliderFloat("##aspect_ratio_slider", &_ratio, 0.5f, 2.f, stringify(smart::as_fraction(_ratio)).c_str()))
    {
        _ratio = make_valid_ratio(_ratio);
        b      = true;
    }
    return b;
}

} // namespace Cool