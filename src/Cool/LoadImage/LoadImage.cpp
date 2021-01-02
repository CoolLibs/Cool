#include "LoadImage.h"

#include <stb_image/stb_image.h>

namespace Cool::LoadImage {

unsigned char* Load(const char* filepath, int* p_width, int* p_height, bool bFlipVertically) {
	stbi_set_flip_vertically_on_load(bFlipVertically ? 1 : 0);
	unsigned char* data = stbi_load(filepath, p_width, p_height, nullptr, 4);
	if (!data)
		Log::Error("Couldn't open file '{}'", filepath);
	return data;
}

void Free(unsigned char* data) {
	stbi_image_free(data);
}

} // namespace Cool::ExportImage