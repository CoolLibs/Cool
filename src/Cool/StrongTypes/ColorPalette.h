#pragma once

#include <algorithms/algorithms.hpp>
#include <op/op.hpp>
#include <vector>
#include "RgbColor.h"

namespace Cool {

struct ColorPalette
    : public op::EqualityComparable<ColorPalette> {
    std::vector<Cool::RgbColor> value{};

    constexpr ColorPalette() = default;
    // This constructor isn't constexpr because std::vector is not constexpr on MacOS
    explicit ColorPalette(const std::vector<Cool::RgbColor>& value)
        : value{value}
    {}

    void remove_color(size_t index)
    {
        assert(index < value.size());
        value.erase(value.begin() + index);
    }

    void add_color(Cool::RgbColor color = {}) { value.push_back(color); }

    void move_color(size_t old_index, size_t new_index)
    {
        algorithms::translocate(
            std::next(value.begin(), old_index),
            std::next(value.begin(), new_index)
        );
    }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Color Palette", value));
    }
};

namespace internal {
inline auto color_palette_array_name(std::string_view name) -> std::string
{
    return fmt::format("{}_", name);
}
} // namespace internal

auto imgui_widget(std::string_view name, Cool::ColorPalette& palette, ImGuiColorEditFlags flags) -> bool;

} // namespace Cool
