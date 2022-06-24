#pragma once

#include <Cool/Image/ImageSizeU.h>

namespace Cool {

class ImageSizeInsideView {
public:
    ImageSizeInsideView(img::Size image_size)
        : _image_size{image_size}
    {
    }

    auto fit_into(img::Size view_size) const -> img::SizeT<float>
    {
        return img::SizeU::fit_into(view_size, _image_size);
    }

private:
    img::Size _image_size;
};

} // namespace Cool
