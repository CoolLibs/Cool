#include "AspectRatio.h"
#include <regex>
#include <smart/smart.hpp>
#include <stringify/stringify.hpp>
#include "Cool/String/String.h"

namespace Cool {

static auto make_valid_ratio(float ratio) -> float
{
    return smart::keep_above(0.001f, ratio);
}

static auto parse_ratio(std::string const& input) -> float
{
    float value = 1.f;

    std::regex const pattern(R"((\d*(\.\d*)?)(/(\d*(\.\d*)?))?(.*))"); // Matches a number and maybe a / and maybe another number
    std::smatch      matches;
    if (std::regex_match(input, matches, pattern))
    {
        if (matches.size() >= 4)
        {
            if (matches[1].matched)
            {
                if (matches[1].str() != "." && !matches[1].str().empty())
                    value = std::stof(matches[1].str());
            }
            if (matches[4].matched)
                if (matches[4].str() != "." && !matches[4].str().empty())
                    value /= std::stof(matches[4].str());
        }
    }

    return make_valid_ratio(value);
}

AspectRatio::AspectRatio(float aspect_ratio)
    : _ratio{make_valid_ratio(aspect_ratio)}
    , _input{string_from_ratio(aspect_ratio)}
{}

void AspectRatio::set(float aspect_ratio)
{
    _ratio = make_valid_ratio(aspect_ratio);
    _input = string_from_ratio(aspect_ratio);
}

auto string_from_ratio(float ratio) -> std::string
{
    smart::Fraction const fraction = smart::as_fraction(ratio);

    bool const fraction_is_small_enough =
        std::abs(fraction.numerator) <= 30
        && std::abs(fraction.denominator) <= 30;

    return fraction_is_small_enough
               ? stringify(fraction)
               : fmt::format("{:.3f}", ratio);
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

    if (ImGui::InputText("##aspect_ratio_input", &_input, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll, nullptr, nullptr, ImDrawFlags_RoundCornersLeft))
    {
        _ratio = parse_ratio(_input);
        b      = true;
    }
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        _input = string_from_ratio(_ratio);
    }

    ImGui::SameLine(0.f, 0.f);

    if (ImGui::BeginCombo("##aspect_ratio_dropdown", "", ImGuiComboFlags_NoPreview, ImDrawFlags_RoundCornersRight))
    {
        for (auto const& ratio : ratios)
        {
            if (ImGui::Selectable(ratio.first, false))
            {
                set(ratio.second);
                b = true;
            }
        }
        ImGui::EndCombo();
    }

    return b;
}

} // namespace Cool

#if COOL_ENABLE_TESTS
#include <doctest/doctest.h>
TEST_CASE("parse_ratio()")
{
    CHECK(doctest::Approx{Cool::parse_ratio("")} == 1.f);
    CHECK(doctest::Approx{Cool::parse_ratio("/")} == 1.f);
    CHECK(doctest::Approx{Cool::parse_ratio(".")} == 1.f);
    CHECK(doctest::Approx{Cool::parse_ratio("3")} == 3.f);
    CHECK(doctest::Approx{Cool::parse_ratio("3.")} == 3.f);
    CHECK(doctest::Approx{Cool::parse_ratio("3./")} == 3.f);
    CHECK(doctest::Approx{Cool::parse_ratio("3/")} == 3.f);
    CHECK(doctest::Approx{Cool::parse_ratio("3/.")} == 3.f);
    CHECK(doctest::Approx{Cool::parse_ratio("3/.5")} == 6.f);
    CHECK(doctest::Approx{Cool::parse_ratio("3/2")} == 1.5f);
    CHECK(doctest::Approx{Cool::parse_ratio("3./2")} == 1.5f);
    CHECK(doctest::Approx{Cool::parse_ratio("3./2.")} == 1.5f);
    CHECK(doctest::Approx{Cool::parse_ratio("3/2.")} == 1.5f);
    CHECK(doctest::Approx{Cool::parse_ratio("0.5")} == 0.5f);
    CHECK(doctest::Approx{Cool::parse_ratio(".5")} == 0.5f);
    CHECK(doctest::Approx{Cool::parse_ratio("/2")} == 0.5f);
    CHECK(doctest::Approx{Cool::parse_ratio("/2.")} == 0.5f);
}
#endif