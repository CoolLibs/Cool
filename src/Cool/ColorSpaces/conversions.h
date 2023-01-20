#pragma once

namespace Cool {

/// All our spaces (except the a and b components of CIELAB) are normalized and take values between 0 and 1.

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
[[nodiscard]] auto HSLuv_from_XYZ(glm::vec3 const& c) -> glm::vec3;
[[nodiscard]] auto XYZ_from_HSLuv(glm::vec3 const& c) -> glm::vec3;
[[nodiscard]] auto HSLuv_from_sRGB(glm::vec3 const& c) -> glm::vec3;
[[nodiscard]] auto sRGB_from_HSLuv(glm::vec3 const& c) -> glm::vec3;
[[nodiscard]] auto HSLuv_from_LinearRGB(glm::vec3 const& c) -> glm::vec3;
[[nodiscard]] auto LinearRGB_from_HSLuv(glm::vec3 const& c) -> glm::vec3;
[[nodiscard]] auto HSLuv_from_CIELAB(glm::vec3 const& c) -> glm::vec3;
[[nodiscard]] auto CIELAB_from_HSLuv(glm::vec3 const& c) -> glm::vec3;

} // namespace Cool