#pragma once

namespace Cool {

struct Point2D {
    glm::vec2 value{};

    friend auto operator==(const Point2D&, const Point2D&) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
        archive(cereal::make_nvp("Point2D", value));
        // #else
        //         (void)archive;
        // #endif
    }
};

auto to_string(Point2D point2D) -> std::string;
auto imgui_widget(std::string_view name, Point2D& point2D, float drag_speed = 0.003f) -> bool;

} // namespace Cool
