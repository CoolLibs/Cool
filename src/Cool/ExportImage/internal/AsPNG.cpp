#include "../AsPNG.h"

#include <stb_image/stb_image_write.h>

namespace Cool::ExportImage {

void AsPNG(std::string_view filepath, int width, int height, void* data, bool bFlipVertically) {
	stbi_flip_vertically_on_write(bFlipVertically ? 1 : 0);
	stbi_write_png(filepath.data(), width, height, 4, data, 0);
}

} // namespace Cool::ExportImage