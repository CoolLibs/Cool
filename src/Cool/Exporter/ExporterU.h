#pragma once
#include <Cool/Image/ImageSize.h>
#include "internal/Polaroid.h"

namespace Cool::ExporterU {

/**
 * @brief Exports an image as PNG
 * 
 * @param size 
 * @param polaroid 
 * @param file_path The name of the file that you want to create
 */
void export_image(ImageSize size, Polaroid polaroid, std::string_view file_path);

} // namespace Cool::ExporterU
