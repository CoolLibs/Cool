#include "Cool/ExportImage/ExportPNG.h"

#include <stb_image/stb_image_write.h>

namespace Cool {

void ExportPng(const char* filepath, int width, int height, void* data) {
	stbi_flip_vertically_on_write(1);
	stbi_write_png(filepath, width, height, 4, data, 0);
}

} // namespace Cool