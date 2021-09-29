#include "ExportImage.h"
#include <stb_image/stb_image_write.h>

namespace Cool::ExportImage {

void as_png(std::string_view file_path, int width, int height, void const* data, bool _flip_vertically)
{
    stbi_flip_vertically_on_write(_flip_vertically ? 1 : 0);
    stbi_write_png(file_path.data(), width, height, 4, data, 0);
}

} // namespace Cool::ExportImage