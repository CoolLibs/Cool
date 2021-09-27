#pragma once

#include "ImageSize.h"

namespace Cool::ImageSizeU {

bool imgui(ImageSize& size);

/**
 * @brief 
 * 
 * @return The aspect ratio of the rectangle, aka width / height
 */
template<typename T>
float aspect_ratio(ImageSizeT<T> size)
{
    return static_cast<float>(size.width()) / static_cast<float>(size.height());
}

/**
 * @return The biggest ImageSizeT<float> that fits into frame and has the same aspect ratio as image.
 */
template<typename T>
ImageSizeT<float> fit_into(ImageSizeT<T> frame, ImageSizeT<T> image)
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

} // namespace Cool::ImageSizeU