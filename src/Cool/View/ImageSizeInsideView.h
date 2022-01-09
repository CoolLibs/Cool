#pragma once

#include <Cool/Image/ImageSizeU.h>

namespace Cool {

class ImageSizeInsideView {
public:
    /**
     * @param image_size 
     * @param needs_to_fit true iff the view and the image are expected to have different aspect ratios and a fitting operation is required 
     */
    ImageSizeInsideView(img::Size image_size, bool needs_to_fit)
        : _image_size{image_size}, _needs_to_fit{needs_to_fit}
    {
    }

    auto fit_into(img::Size view_size) const -> img::SizeT<float>
    {
        return _needs_to_fit
                   ? img::SizeU::fit_into(view_size, _image_size)
                   : static_cast<img::SizeT<float>>(view_size);
    }

private:
    img::Size _image_size;
    bool      _needs_to_fit;
};

} // namespace Cool
