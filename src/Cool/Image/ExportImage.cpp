#include "ExportImage.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <stb_image/stb_image_write.h>

namespace Cool::ExportImage {

void as_png(std::string_view file_path, const ImageData& image, bool flip_vertically)
{
    as_png(file_path, image.size.width(), image.size.height(), image.data.get(), flip_vertically);
}

void as_png(std::string_view file_path, int width, int height, void const* data, bool flip_vertically)
{
    if (File::create_folders_for_file_if_they_dont_exist(file_path)) {
        stbi_flip_vertically_on_write(flip_vertically ? 1 : 0);
        stbi_write_png(file_path.data(), width, height, 4, data, 0);
    }
    else {
        Log::ToUser::warn("ExportImage::as_png", "Couldn't export because folder creation failed !");
    }
}

} // namespace Cool::ExportImage