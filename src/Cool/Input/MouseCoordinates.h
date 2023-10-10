#pragma once

namespace Cool {

template<typename T>
concept MouseCoordinates = requires(T coords) {
                               // clang-format off
    { coords } -> std::convertible_to<glm::vec2>;
                               // clang-format on
                           };

/// The coordinate system used by Dear ImGui (aka the one used by ImGui::GetMousePos(), etc.)
class ImGuiCoordinates : public glm::vec2 {
public:
    template<typename... Args>
    explicit ImGuiCoordinates(Args... args)
        : glm::vec2{std::forward<Args>(args)...}
    {
    }

    ImGuiCoordinates(ImVec2 v) // NOLINT(*explicit-constructor, *explicit-conversions)
        : glm::vec2{v.x, v.y}
    {}

    ImGuiCoordinates()
        : glm::vec2{0.f}
    {}
};

/// Coordinates relative to a specific View (more precisely: to the image inside that view).
/// x-axis goes from -image_aspect_ratio to +image_aspect_ratio (-image_aspect_ratio on the left of the image inside the View)
/// y-axis goes from -1 to +1 (-1 at the bottom of the image inside the View)
class ViewCoordinates : public glm::vec2 {
public:
    template<typename... Args>
    explicit ViewCoordinates(Args... args)
        : glm::vec2{std::forward<Args>(args)...}
    {
    }

    ViewCoordinates()
        : glm::vec2{0.f}
    {}
};

} // namespace Cool