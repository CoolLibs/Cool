#pragma once

namespace Cool {

struct TimeSpeed {
    float value{};

    friend auto operator<=>(TimeSpeed const&, TimeSpeed const&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    auto save_minimal(Archive const&) const -> float
    {
        return value;
    }
    template<class Archive>
    void load_minimal(Archive const&, float const& val)
    {
        value = val;
    }
};

auto to_string(TimeSpeed time_speed) -> std::string;

} // namespace Cool