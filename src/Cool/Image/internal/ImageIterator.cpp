#include "ImageIterator.h"
#include "../Image.h"

namespace Cool {

ImageIterator::ImageIterator(Image& image, unsigned int x, unsigned int y)
	: m_image(image), m_x(x), m_y(y)
{}

Pixel ImageIterator::operator*() {
	return Pixel(m_image, m_x, m_y);
}

void ImageIterator::operator++() {
	m_x++;
	if (m_x == m_image.width()) {
		m_x = 0;
		m_y++;
	}
}

bool ImageIterator::operator!=(ImageIterator& it) {
	return m_x != it.m_x
		|| m_y != it.m_y;
}

} // namespace Cool