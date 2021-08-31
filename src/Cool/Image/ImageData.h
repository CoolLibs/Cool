#pragma once

#include "ImageSize.h"

namespace Cool {

struct ImageData {
    ImageSize                  size;
    int                        nb_of_channels;
    std::unique_ptr<uint8_t[]> data;

    ImageSize::DataType width() const { return size.width(); }
    ImageSize::DataType height() const { return size.height(); }

    /**
     * @brief Loads an image from a file using stb_image (https://github.com/nothings/stb/blob/master/stb_image.h)
     * 
     * @param file_path The path to the image : something like "icons/myImage.png"
     * @param bFlipVertically By default we use the OpenGL convention : the first row will be the bottom of the image. You can set bFlipVertically to false if you want the first row to be the top of the image
     * @return An array of unsigned char (wrapped in a unique_ptr) representing the image, with 4 channels per pixel (RGBA). The pixels are written sequentially, row after row. Something like [255, 200, 100, 255, 120, 30, 80, 255, ...] where (255, 200, 100, 255) would be the pixel on 1st row and 1st column and (120, 30, 80, 255) the pixel on 1st row and 2nd column and so on
     */
    ImageData(std::string_view file_path, int nb_of_channels = 4, bool flip_vertically = true);

    ImageData(ImageSize size, int nb_of_channels, std::unique_ptr<uint8_t[]>&& data)
        : size{size}, nb_of_channels{nb_of_channels}, data{std::move(data)}
    {
    }

    /**
     * @brief data is a pointer to the beginning of an array (stored as a pointer because this is how stb_image does things).
     * This give you the number of elements in the array.
     * 
     * @return size_t 
     */
    size_t data_array_size() const { return width() * height() * nb_of_channels; }
};

} // namespace Cool