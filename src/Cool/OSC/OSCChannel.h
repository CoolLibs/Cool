#pragma once

namespace Cool {

struct OSCChannel {
    std::string name{""};

    friend auto operator==(OSCChannel const&, OSCChannel const&) -> bool = default;
    friend auto operator<(OSCChannel const& a, OSCChannel const& b) -> bool { return a.name < b.name; }

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