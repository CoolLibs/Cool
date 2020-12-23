#include "../Image.h"

#include <Cool/ExportImage/ExportPNG.h>
#include <Cool/File/File.h>

namespace Cool {

Image::Image(unsigned int width, unsigned int height)
	: m_width(width), m_height(height), m_aspectRatio(static_cast<float>(width) / static_cast<float>(height))
{
	m_pixelColors.resize(m_width * m_height);
}

void Image::saveAs(const char* filepath) {
	Cool::File::CreateFoldersForFileIfDoesntExist(filepath);
	Cool::ExportImage::AsPNG(filepath, m_width, m_height, m_pixelColors.data());
}

ImageIterator Image::begin() {
	return ImageIterator(*this, 0, 0);
}

ImageIterator Image::end() {
	return ImageIterator(*this, 0, m_height);
}

} // namespace Cool