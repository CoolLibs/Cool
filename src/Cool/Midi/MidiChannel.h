#pragma once

namespace Cool {

enum class MidiChannelKind {
    Slider,
    ButtonToggle,
    ButtonSelector, // Toggle, and only keep the last one that was pressed
    ButtonWhilePressed,

    COUNT, // Must be last
};

auto user_facing_string(MidiChannelKind kind) -> const char*;
auto imgui_widget(const char* label, MidiChannelKind& kind) -> bool;

struct MidiChannel {
    int             index{};
    MidiChannelKind kind{};

    friend auto operator<=>(MidiChannel const&, MidiChannel const&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Index", index),
            cereal::make_nvp("Kind", kind)
        );
    }
};

} // namespace Cool