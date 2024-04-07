#include "SaveImage.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>

namespace Cool::ImageU {

void save_png(
    std::filesystem::path file_path,
    const img::Image&     image,
    bool                  flip_vertically
)
{
    Cool::ImageU::save_png(file_path, image.width(), image.height(), image.data(), image.channels_count(), flip_vertically);
}

void save_png(
    std::filesystem::path file_path,
    img::Size::DataType   width,
    img::Size::DataType   height,
    const void*           data,
    int                   channels_count,
    bool                  flip_vertically
)
{
    if (File::create_folders_for_file_if_they_dont_exist(file_path))
    {
        img::save_png(file_path, width, height, data, channels_count, flip_vertically);
    }
    else
    {
        Log::ToUser::warning("ImageU::save_png", "Couldn't export because folder creation failed!");
    }
}

} // namespace Cool::ImageU