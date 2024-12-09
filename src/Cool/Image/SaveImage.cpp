#include "SaveImage.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>

namespace Cool::ImageU {

auto save_png(
    std::filesystem::path const& file_path,
    img::Image const&            image,
    img::SaveOptions const&      options
) -> bool
{
    return Cool::ImageU::save_png(file_path, image.size().width(), image.size().height(), image.data(), image.channels_count(), image.row_order(), options);
}

auto save_png(
    std::filesystem::path const& file_path,
    img::Size::DataType          width,
    img::Size::DataType          height,
    const void*                  data,
    size_t                       channels_count,
    img::FirstRowIs              row_order,
    img::SaveOptions const&      options
) -> bool
{
    if (!File::create_folders_for_file_if_they_dont_exist(file_path))
    {
        Log::ToUser::warning("Export", "Failed to export because folder creation failed!");
        return false;
    }
    if (!img::save_png(file_path, width, height, data, channels_count, row_order, options))
    {
        if (!(options.cancel && options.cancel->load())) // Don't log warning if the export has been intentionally cancelled
            Log::ToUser::warning("Export", "Failed to save image");
        return false;
    }
    return true;
}

} // namespace Cool::ImageU