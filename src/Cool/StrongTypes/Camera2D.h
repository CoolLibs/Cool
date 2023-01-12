#pragma once

namespace Cool {

struct Camera2D {
    glm::vec2 translation{};
    float     zoom{};

    auto transform_matrix() const -> glm::mat3;

    friend auto operator==(Camera2D const& a, Camera2D const& b) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Translation", translation),
            cereal::make_nvp("Zoom", zoom)
        );
    }
};

auto to_string(Camera2D const&) -> std::string;
auto imgui_widget(std::string_view name, Camera2D&) -> bool;

} // namespace Cool