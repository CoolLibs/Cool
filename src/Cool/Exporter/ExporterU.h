#pragma once
#include <img/img.hpp>
#include "internal/Polaroid.h"

namespace Cool::ExporterU {

/// Exports an image as PNG
[[nodiscard]] auto export_image(img::Size size, Time time, Time delta_time, Polaroid const& polaroid, std::filesystem::path const& file_path) -> bool;

void notification_after_export_success(std::filesystem::path const& path, bool is_video);
void notification_after_export_failure();
void notification_after_export_interrupted();

} // namespace Cool::ExporterU
