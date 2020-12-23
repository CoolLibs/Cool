#include "Pixel.h"

#include "../Image.h"

namespace Cool {

Pixel::Pixel(Image& image, unsigned int x, unsigned int y)
	: m_image(image), m_x(x), m_y(y)
{}

Color& Pixel::color() { return m_image.colorAt(m_x, m_y); }

float Pixel::x() {
	return (static_cast<float>(m_x) / static_cast<float>(m_image.width() - 1) * 2.0f - 1.0f) * m_image.aspectRatio();
}
float Pixel::y() {
	return  static_cast<float>(m_y) / static_cast<float>(m_image.height() - 1) * 2.0f - 1.0f;
}

} // namespace Cool