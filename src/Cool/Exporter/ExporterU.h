#pragma once
#include <img/img.hpp>
#include "internal/Polaroid.h"

namespace Cool::ExporterU {

/**
 * @brief Exports an image as PNG
 *
 * @param file_path The name of the file that you want to create
 */
void export_image(img::Size size, Time time, Time delta_time, Polaroid const& polaroid, std::filesystem::path const& file_path);

} // namespace Cool::ExporterU
