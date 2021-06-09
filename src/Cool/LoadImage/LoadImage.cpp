#include "LoadImage.h"

#include <stb_image/stb_image.h>

namespace Cool::LoadImage {

std::unique_ptr<unsigned char> load(std::string_view filepath, int* p_width, int* p_height, bool flip_vertically) {
	stbi_set_flip_vertically_on_load(flip_vertically ? 1 : 0);
	unsigned char* data = stbi_load(filepath.data(), p_width, p_height, nullptr, 4);
	if (!data) {
		Log::error("[LoadImage::load] Couldn't open file \"{}\"", filepath);
	}
	return std::unique_ptr<unsigned char>(data);
}

} // namespace Cool::ExportImage