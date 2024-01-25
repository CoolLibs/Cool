#pragma once

namespace Cool {

struct OSCChannel {
    std::string route{""};

    friend auto operator==(OSCChannel const&, OSCChannel const&) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Route", route));
    }
};

} // namespace Cool