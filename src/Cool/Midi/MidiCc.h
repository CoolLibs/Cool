#pragma once

namespace Cool {

    struct MidiCc{
// TODO(Midi) int index du slider / button
int index{};
    friend auto operator==(MidiCc const& a, MidiCc const& b) -> bool = default;

    
private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Index", index));
    }
    };

}