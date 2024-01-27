#pragma once

namespace Cool {

struct MidiChannel {
    int index{};

    friend auto operator<=>(MidiChannel const&, MidiChannel const&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Index", index));
    }
};

} // namespace Cool