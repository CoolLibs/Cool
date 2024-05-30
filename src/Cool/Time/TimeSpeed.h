#pragma once

namespace Cool {

struct TimeSpeed {
    double value{};

    friend auto operator<=>(TimeSpeed const&, TimeSpeed const&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    auto save_minimal(Archive const&) const -> double
    {
        return value;
    }
    template<class Archive>
    void load_minimal(Archive const&, double const& val)
    {
        value = val;
    }
};

auto to_string(TimeSpeed time_speed) -> std::string;
auto imgui_widget(const char* label, TimeSpeed* time_speed, float v_speed = 0.0005f, double v_min = -DBL_MAX, double v_max = DBL_MAX, ImGuiSliderFlags flags = 0) -> bool;

} // namespace Cool