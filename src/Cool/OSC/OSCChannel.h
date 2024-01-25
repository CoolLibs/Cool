#pragma once

namespace Cool {

struct OSCChannel {
    std::string name{""};

    friend auto operator<=>(OSCChannel const&, OSCChannel const&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Name", name));
    }
};

} // namespace Cool