#pragma once

namespace Cool {

[[nodiscard]] auto LinearRGB_from_sRGB(glm::vec3) -> glm::vec3;
[[nodiscard]] auto sRGB_from_LinearRGB(glm::vec3) -> glm::vec3;
[[nodiscard]] auto XYZ_from_LinearRGB(glm::vec3 const&) -> glm::vec3;
[[nodiscard]] auto XYZ_from_sRGB(glm::vec3 const&) -> glm::vec3;
[[nodiscard]] auto CIELAB_from_XYZ(glm::vec3 const&) -> glm::vec3;
[[nodiscard]] auto CIELAB_from_sRGB(glm::vec3 const&) -> glm::vec3;
[[nodiscard]] auto CIELAB_from_LinearRGB(glm::vec3 const&) -> glm::vec3;
[[nodiscard]] auto XYZ_from_CIELAB(glm::vec3 const&) -> glm::vec3;
[[nodiscard]] auto LinearRGB_from_XYZ(glm::vec3 const&) -> glm::vec3;
[[nodiscard]] auto sRGB_from_XYZ(glm::vec3 const&) -> glm::vec3;
[[nodiscard]] auto sRGB_from_CIELAB(glm::vec3 const&) -> glm::vec3;
[[nodiscard]] auto LinearRGB_from_CIELAB(glm::vec3 const&) -> glm::vec3;

} // namespace Cool