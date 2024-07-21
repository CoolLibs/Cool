#pragma once

#include <algorithms/algorithms.hpp>
#include <op/op.hpp>
#include <vector>
#include "Color.h"

namespace Cool {

struct ColorPalette
    : public op::EqualityComparable<ColorPalette> {
    std::vector<Color> value{};

    constexpr ColorPalette() = default;
    // This constructor isn't constexpr because std::vector is not constexpr on MacOS
    explicit ColorPalette(const std::vector<Color>& value)
        : value{value}
    {}

    void remove_color(size_t index)
    {
        assert(index < value.size());
        value.erase(value.begin() + static_cast<decltype(value)::difference_type>(index));
    }

    void add_color(Color color = {}) { value.push_back(color); }

    void move_color(size_t old_index, size_t new_index)
    {
        algorithms::translocate(
            std::next(value.begin(), static_cast<decltype(value)::difference_type>(old_index)),
            std::next(value.begin(), static_cast<decltype(value)::difference_type>(new_index))
        );
    }

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(ser20::make_nvp("Color Palette", value));
    }
};

namespace internal {
inline auto color_palette_array_name(std::string_view name) -> std::string
{
    return fmt::format("{}_", name);
}
} // namespace internal

auto imgui_widget(std::string_view name, ColorPalette& palette, ImGuiColorEditFlags flags) -> bool;

} // namespace Cool
