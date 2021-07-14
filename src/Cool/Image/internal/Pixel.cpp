#include "Pixel.h"
#include "../Image.h"

namespace Cool {

Pixel::Pixel(Image& image, unsigned int x, unsigned int y)
    : _image(image), _x(x), _y(y)
{
}

Color& Pixel::color()
{
    return _image.color_at(_x, _y);
}

float Pixel::x()
{
    return (static_cast<float>(_x) / static_cast<float>(_image.width() - 1) * 2.f - 1.f) * _image.aspect_ratio();
}

float Pixel::y()
{
    return static_cast<float>(_y) / static_cast<float>(_image.height() - 1) * 2.f - 1.f;
}

} // namespace Cool