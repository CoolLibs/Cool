#include "Color.h"

namespace Cool {

Color::Color() {
	m_channels[0] = 0;
	m_channels[1] = 0;
	m_channels[2] = 0;
	m_channels[3] = 255;
}

void Color::set(unsigned char r, unsigned char g, unsigned char b) {
	m_channels[0] = r;
	m_channels[1] = g;
	m_channels[2] = b;
}

void Color::set(float r, float g, float b) {
	set(
		static_cast<unsigned char>(std::clamp(r * 255.f, 0.f, 255.f)),
		static_cast<unsigned char>(std::clamp(g * 255.f, 0.f, 255.f)),
		static_cast<unsigned char>(std::clamp(b * 255.f, 0.f, 255.f))
	);
}

} // namespace Cool