#pragma once
#include <img/img.hpp>

namespace Cool::ImageU {

/// Saves an image as PNG
/// Returns true iff the operation succeeded
/// @param file_path The destination path for the image: something like "out/myImage.png". The folders will be created if they don't already exist.
[[nodiscard]] auto save_png(
    std::filesystem::path const& file_path,
    img::Image const&            image,
    img::SaveOptions const&      options = {}
) -> bool;

/// Saves an image as PNG
/// Returns true iff the operation succeeded
/// @param file_path The destination path for the image: something like "out/myImage.png". The folders will be created if they don't already exist.
/// @param width The width in pixels of the image represented by data.
/// @param height The height in pixels of the image represented by data.
/// @param data An array of uint8_t representing the image. The pixels should be written sequentially, row after row. Something like [255, 200, 100, 255, 120, 30, 80, 255, ...] where (255, 200, 100, 255) would be the first pixel and (120, 30, 80, 255) the second pixel and so on.
/// @param channels_count The number of channels per pixel, e.g. 4 if the format is RGBA.
[[nodiscard]] auto save_png(
    std::filesystem::path const& file_path,
    img::Size::DataType          width,
    img::Size::DataType          height,
    const void*                  data,
    size_t                       channels_count,
    img::FirstRowIs              row_order,
    img::SaveOptions const&      options = {}
) -> bool;

} // namespace Cool::ImageU