#include "Color.h"

namespace Cool {

Color::Color()
{
    _channels[0] = 0;
    _channels[1] = 0;
    _channels[2] = 0;
    _channels[3] = 255;
}

void Color::set(unsigned char r, unsigned char g, unsigned char b)
{
    _channels[0] = r;
    _channels[1] = g;
    _channels[2] = b;
}

void Color::set(float r, float g, float b)
{
    set(
        static_cast<unsigned char>(std::clamp(r * 255.f, 0.f, 255.f)),
        static_cast<unsigned char>(std::clamp(g * 255.f, 0.f, 255.f)),
        static_cast<unsigned char>(std::clamp(b * 255.f, 0.f, 255.f)));
}

} // namespace Cool