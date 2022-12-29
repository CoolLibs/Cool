#pragma once

namespace Cool {

class Color {
public:
    Color() = default;
    static auto from_srgb(glm::vec3 const& srgb) -> Color;

    [[nodiscard]] auto as_srgb() const -> glm::vec3;
    [[nodiscard]] auto as_linear_rgb() const -> glm::vec3;
    [[nodiscard]] auto as_cielab() const -> glm::vec3;

    friend auto operator==(Color const&, Color const&) -> bool = default;

private:
    glm::vec3 _srgb{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("sRGB", _srgb));
    }
};

auto to_string(Color const&) -> std::string;
auto imgui_widget(std::string_view name, Color&, ImGuiColorEditFlags) -> bool;

} // namespace Cool
