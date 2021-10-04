#pragma once

#include "Pixel.h"

namespace Cool {

class CpuImage;

class CpuImageIterator {
public:
    CpuImageIterator(CpuImage& image, unsigned int x, unsigned int y);

    Pixel operator*() const;
    void  operator++();
    bool  operator!=(CpuImageIterator& it) const;

private:
    CpuImage&    _image;
    unsigned int _x;
    unsigned int _y;
};

} // namespace Cool