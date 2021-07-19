#pragma once

#include "Image.h"

namespace Cool {

class IRenderTarget {
public:
    virtual Image download_pixels() const = 0;
};

} // namespace Cool