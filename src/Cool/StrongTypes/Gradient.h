#pragma once

#include <Cool/Serialization/ImGuiSerialization.h>
#include <cereal/types/list.hpp>
#include <imgui_gradient/imgui_gradient.hpp>

namespace Cool {
struct Gradient {
    ImGG::GradientWidget value{};
    ImGG::WrapMode       wrap_mode{ImGG::WrapMode::MirrorRepeat};

    friend auto operator==(const Gradient& a, const Gradient& b) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
        const auto wrap_mode_size_t          = static_cast<size_t>(wrap_mode);
        const auto interpolation_mode_size_t = static_cast<size_t>(value.gradient().interpolation_mode());
        archive(
            cereal::make_nvp("Gradient", value.gradient().get_marks()),
            cereal::make_nvp("Wrap Mode", wrap_mode_size_t),
            cereal::make_nvp("Interpolation Mode", interpolation_mode_size_t)
        );
        // #else
        //         (void)archive;
        // #endif
    }
    friend class cereal::access;
    template<class Archive>
    void load(Archive& archive)
    {
        // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
        std::list<ImGG::Mark> gradient_loaded{};
        size_t                wrap_mode_size_t{};
        size_t                interpolation_mode_size_t{};
        archive(gradient_loaded, wrap_mode_size_t, interpolation_mode_size_t);
        value.gradient()                      = ImGG::Gradient{gradient_loaded};
        wrap_mode                             = static_cast<ImGG::WrapMode>(wrap_mode_size_t);
        value.gradient().interpolation_mode() = static_cast<ImGG::Interpolation>(interpolation_mode_size_t);
        // #else
        //         (void)archive;
        // #endif
    }
};

inline auto to_string(Cool::Gradient) -> std::string
{
    return "Gradient [...]";
}

inline auto imgui_widget(std::string_view name, Cool::Gradient& gradient, bool should_use_a_random_color_for_the_new_marks, ImGuiColorEditFlags flags) -> bool
{
    auto modified = gradient.value.widget(
        name.data(),
        {
            .flags                                       = ImGG::Flag::NoResetButton,
            .color_edit_flags                            = flags,
            .should_use_a_random_color_for_the_new_marks = should_use_a_random_color_for_the_new_marks,
            .plus_button_text                            = "+",
            .minus_button_text                           = "-",
        }
    );
    modified |= ImGG::interpolation_mode_widget("Interpolation Mode", &gradient.value.gradient().interpolation_mode());
    ImGui::SameLine();
    modified |= ImGG::wrap_mode_widget("Wrap Mode", &gradient.wrap_mode);
    ImGui::SameLine();
    if (ImGui::Button("Spread marks evenly"))
    {
        gradient.value.gradient().spread_marks_evenly();
        modified = true;
    }
    return modified;
}

namespace internal {
inline auto gradient_marks_array_name(std::string_view name) -> std::string
{
    return fmt::format("{}_", name);
}
} // namespace internal

} // namespace Cool

namespace cereal {

template<class Archive>
void serialize(Archive& archive, ImGG::Mark& mark)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(
        cereal::make_nvp("Mark position", mark.position),
        cereal::make_nvp("Mark color", mark.color)
    );
    // #else
    //     (void)archive;
    //     (void)mark;
    // #endif
}

template<class Archive>
void save(Archive& archive, ImGG::RelativePosition const& position)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(cereal::make_nvp("Relative position", position.get()));
    // #else
    //     (void)archive;
    //     (void)position;
    // #endif
}

template<class Archive>
void load(Archive& archive, ImGG::RelativePosition& position)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    float position_loaded{};
    archive(position_loaded);
    position = ImGG::RelativePosition{position_loaded};
    // #else
    //     (void)archive;
    //     (void)position;
    // #endif
}

} // namespace cereal