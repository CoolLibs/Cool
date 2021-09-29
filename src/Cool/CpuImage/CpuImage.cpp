#include "CpuImage.h"
#include <Cool/File/File.h>
#include <Cool/Image/ExportImage.h>

namespace Cool {

CpuImage::CpuImage(unsigned int width, unsigned int height)
    : _width(width), _height(height), _aspect_ratio(static_cast<float>(width) / static_cast<float>(height))
{
    _pixel_colors.resize(_width * _height);
}

void CpuImage::save_as(const char* filepath)
{
    Cool::ExportImage::as_png(filepath, _width, _height, _pixel_colors.data());
}

CpuImageIterator CpuImage::begin()
{
    return CpuImageIterator(*this, 0, 0);
}

CpuImageIterator CpuImage::end()
{
    return CpuImageIterator(*this, 0, _height);
}

} // namespace Cool