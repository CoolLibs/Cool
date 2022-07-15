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
    constexpr explicit ColorPalette(const std::vector<Cool::RgbColor>& value)
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

auto imgui_widget(std::string_view name, Cool::ColorPalette& palette, ImGuiColorEditFlags flags) -> bool;

} // namespace Cool
