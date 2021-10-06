#pragma once

#include <Cool/Image/ImageSizeU.h>

namespace Cool {

class ImageSizeInsideView {
public:
    /**
     * @param image_size 
     * @param needs_to_fit true iff the view and the image are expected to have different aspect ratios and a fitting operation is required 
     */
    ImageSizeInsideView(ImageSize image_size, bool needs_to_fit)
        : _image_size{image_size}, _needs_to_fit{needs_to_fit}
    {
    }

    auto fit_into(ImageSize view_size) const -> ImageSizeT<float>
    {
        return _needs_to_fit
                   ? ImageSizeU::fit_into(view_size, _image_size)
                   : static_cast<ImageSizeT<float>>(view_size);
    }

private:
    ImageSize _image_size;
    bool      _needs_to_fit;
};

} // namespace Cool
