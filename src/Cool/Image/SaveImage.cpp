#include "SaveImage.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>

namespace Cool::ImageU {

void save_png(
    std::filesystem::path const& file_path,
    img::Image const&            image,
    bool                         flip_vertically
)
{
    Cool::ImageU::save_png(file_path, image.size().width(), image.size().height(), image.data(), image.channels_count(), flip_vertically);
}

void save_png(
    std::filesystem::path const& file_path,
    img::Size::DataType          width,
    img::Size::DataType          height,
    const void*                  data,
    int                          channels_count,
    bool                         flip_vertically
)
{
    if (!File::create_folders_for_file_if_they_dont_exist(file_path))
    {
        Log::ToUser::warning("Export", "Failed to export because folder creation failed!");
        return;
    }
    if (!img::save_png(file_path, width, height, data, channels_count, flip_vertically))
        Log::ToUser::warning("Export", "Failed to save image");
}

} // namespace Cool::ImageU