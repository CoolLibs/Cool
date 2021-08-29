#pragma once

namespace Cool {

/**
 * @brief Represents the width and height of an axis-aligned rectangle.
 * width and height are guaranteed to be >= 1
 * 
 * @tparam T The type of width and height
 */
template<typename T>
class ImageSizeT {
public:
    /**
     * @brief The type of width and height
     * 
     */
    using DataType = T;

    ImageSizeT() = default;
    ImageSizeT(DataType w, DataType h)
        : _width{w}, _height{h}
    {
        assert(w >= 1 && h >= 1);
    }
    friend bool operator==(const ImageSizeT&, const ImageSizeT&) = default;
    template<typename U>
    explicit operator ImageSizeT<U>() const
    {
        return ImageSizeT<U>{static_cast<U>(_width), static_cast<U>(_height)};
    }

    DataType width() const { return _width; }
    DataType height() const { return _height; }

    /**
     * @brief Sets the width
     * @param w must be >= 1
     */
    void set_width(DataType w)
    {
        assert(w >= 1);
        _width = w;
    }

    /**
     * @brief Sets the height
     * @param h must be >= 1
     */
    void set_height(DataType h)
    {
        assert(h >= 1);
        _height = h;
    }

private:
    DataType _width  = 1;
    DataType _height = 1;
};

using ImageSize = ImageSizeT<uint32_t>;

namespace ImageSizeU {

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
ImageSizeT<float> fit_into(ImageSize frame, ImageSize image);

} // namespace ImageSizeU

} // namespace Cool