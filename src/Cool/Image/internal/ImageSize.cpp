#include "../ImageSize.h"

namespace Cool {

ImageSizeT<float> ImageSizeU::fit_into(ImageSize frame, ImageSize image)
{
    auto image_aspect = aspect_ratio(image);
    auto frame_aspect = aspect_ratio(frame);
    if (image_aspect < frame_aspect) {
        return {
            static_cast<float>(frame.height()) * image_aspect,
            static_cast<float>(frame.height())};
    }
    else {
        return {
            static_cast<float>(frame.width()),
            static_cast<float>(frame.width()) / image_aspect};
    }
}

} // namespace Cool