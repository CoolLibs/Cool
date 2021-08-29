#pragma once

namespace Cool {

/**
 * @brief Represents the width and height of an axis-aligned rectangle.
 * width and height are guaranteed to be >= 1
 * 
 */
class ImageSize {
public:
    /**
     * @brief The type of width and height
     * 
     */
    using DataType = uint32_t;

    ImageSize() = default;
    ImageSize(DataType w, DataType h)
        : _width{w}, _height{h}
    {
        assert(w >= 1 && h >= 1);
    }

    DataType width() const { return _width; }
    DataType height() const { return _height; }

    /**
     * @brief 
     * 
     * @return The aspect ratio of the rectangle, aka width / height 
     */
    float aspect_ratio() const { return static_cast<float>(_width) / static_cast<float>(_height); }

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

} // namespace Cool