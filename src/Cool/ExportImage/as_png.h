#pragma once

namespace Cool::ExportImage {

/**
 * @brief Exports an image as PNG using stb_image_write (https://github.com/nothings/stb/blob/master/stb_image_write.h)
 * 
 * @param file_path The destination path for the image : something like "out/myImage.png". /!\ Warning : If folders in the path don't already exist they won't be created. You can use Cool::File::create_folders_for_file_if_they_dont_exist() to solve this
 * @param width The width in pixels of the image represented by data
 * @param height The height in pixels of the image represented by data
 * @param data An array of unsigned char representing the image. We expect 4 channels per pixel (RGBA) and the pixels should be written sequentially, row after row. Something like [255, 200, 100, 255, 120, 30, 80, 255, ...] where (255, 200, 100, 255) would be the pixel on 1st row and 1st column and (120, 30, 80, 255) the pixel on 1st row and 2nd column and so on
 * @param _flip_vertically By default we use the OpenGL convention : the first row should be the bottom of the image. You can set _flip_vertically to false if your first row is at the top of the image
 */
void as_png(std::string_view file_path, int width, int height, void const* data, bool _flip_vertically = true);

} // namespace Cool::ExportImage