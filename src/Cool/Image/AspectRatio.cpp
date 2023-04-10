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

static auto imgui_string(float ratio) -> std::string
{
    const smart::Fraction fraction = smart::as_fraction(ratio);

    const bool fraction_is_small_enough =
        std::abs(fraction.numerator) <= 30
        && std::abs(fraction.denominator) <= 30;

    return fraction_is_small_enough
               ? stringify(fraction)
               : "%.3f";
}

auto AspectRatio::imgui(float width) -> bool
{
    bool b = false;

    static constexpr std::array ratios = {
        std::make_pair("     16 / 9     ", 16.f / 9.f),
        std::make_pair("      3 / 2     ", 3.f / 2.f),
        std::make_pair("      4 / 3     ", 4.f / 3.f),
        std::make_pair("      1 / 1     ", 1.f),
        std::make_pair("      9 / 16    ", 9.f / 16.f),
        std::make_pair("      2 / 3     ", 2.f / 3.f),
        std::make_pair("      3 / 4     ", 3.f / 4.f),
    };

    if (width != 0.f)
        ImGui::SetNextItemWidth(width);
    if (ImGui::InputFloat("##aspect_ratio_input", &_ratio, 0.f, 0.f, imgui_string(_ratio).c_str(), ImGuiInputTextFlags_CharsScientific, ImDrawFlags_RoundCornersLeft))
    {
        _ratio = make_valid_ratio(_ratio);
        b      = true;
    }

    ImGui::SameLine(0.f, 0.f);

    if (ImGui::BeginCombo("##aspect_ratio_dropdown", "", ImGuiComboFlags_NoPreview, ImDrawFlags_RoundCornersRight))
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

    return b;
}

} // namespace Cool