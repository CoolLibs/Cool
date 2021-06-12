#include "../Image.h"

#include <Cool/ExportImage/AsPNG.h>
#include <Cool/File/File.h>

namespace Cool {

Image::Image(unsigned int width, unsigned int height)
	: _width(width), _height(height), _aspect_ratio(static_cast<float>(width) / static_cast<float>(height))
{
	_pixel_colors.resize(_width * _height);
}

void Image::save_as(const char* filepath) {
	Cool::File::create_folders_for_file_if_they_dont_exist(filepath);
	Cool::ExportImage::as_png(filepath, _width, _height, _pixel_colors.data());
}

ImageIterator Image::begin() {
	return ImageIterator(*this, 0, 0);
}

ImageIterator Image::end() {
	return ImageIterator(*this, 0, _height);
}

} // namespace Cool