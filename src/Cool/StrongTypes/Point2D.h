#pragma once

namespace Cool {

struct Point2D {
    glm::vec2 value{};

    friend auto operator==(const Point2D&, const Point2D&) -> bool = default;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(ser20::make_nvp("Point2D", value));
    }
};

auto to_string(Point2D point2D) -> std::string;
auto imgui_widget(std::string_view name, Point2D& point2D, float drag_speed = 0.003f) -> bool;

} // namespace Cool
