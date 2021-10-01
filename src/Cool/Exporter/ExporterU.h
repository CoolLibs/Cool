#pragma once
#include <Cool/Image/ImageSize.h>
#include "internal/Polaroid.h"

namespace Cool::ExporterU {

/**
 * @brief Exports an image as PNG
 * 
 * @param size 
 * @param time 
 * @param polaroid 
 * @param file_path The name of the file that you want to create
 */
void export_image(ImageSize size, float time, Polaroid polaroid, std::string_view file_path);

} // namespace Cool::ExporterU
