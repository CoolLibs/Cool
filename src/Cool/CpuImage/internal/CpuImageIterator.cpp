#include "CpuImageIterator.h"
#include "../CpuImage.h"

namespace Cool {

CpuImageIterator::CpuImageIterator(CpuImage& image, unsigned int x, unsigned int y)
    : _image(image), _x(x), _y(y)
{
}

Pixel CpuImageIterator::operator*() const
{
    return Pixel(_image, _x, _y);
}

void CpuImageIterator::operator++()
{
    _x++;
    if (_x == _image.width())
    {
        _x = 0;
        _y++;
    }
}

bool CpuImageIterator::operator!=(CpuImageIterator& it) const
{
    return _x != it._x || _y != it._y;
}

} // namespace Cool