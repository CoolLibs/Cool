#pragma once
#include <img/img.hpp>
#include "ImGuiNotify/ImGuiNotify.hpp"
#include "Polaroid.hpp"

namespace Cool::ExporterU {

/// Exports an image as PNG
void export_image_using_a_task(img::Size size, Time time, Time delta_time, Polaroid const& polaroid, std::filesystem::path const& file_path);

[[nodiscard]] auto notification_after_export_success(std::filesystem::path const& path, bool is_video) -> ImGuiNotify::Notification;
[[nodiscard]] auto notification_after_export_failure(std::string const& error_message, bool is_video) -> ImGuiNotify::Notification;
[[nodiscard]] auto notification_after_image_export_canceled() -> ImGuiNotify::Notification;
[[nodiscard]] auto notification_after_video_export_canceled(std::filesystem::path const& path) -> ImGuiNotify::Notification;

} // namespace Cool::ExporterU
