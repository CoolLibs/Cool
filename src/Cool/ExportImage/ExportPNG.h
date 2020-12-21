#pragma once

namespace Cool::ExportImage {

/// <summary>
/// Exports an image as PNG using stb_image_write (https://github.com/nothings/stb/blob/master/stb_image_write.h)
/// </summary>
/// <param name="filepath">The destination path for the image : something like "out/myImage.png". /!\ Warning : If folders in the path don't already exist they won't be created. You can use Cool::File::CreateFoldersForFileIfDoesntExist() to solve this.</param>
/// <param name="width">The width in pixels of the image represented by <i>data</i></param>
/// <param name="height">The height in pixels of the image represented by <i>data</i></param>
/// <param name="data">An array of unsigned char representing the image. We expect 4 channels per pixel (RGBA) and the pixels should be written sequentially, row after row.
/// Something like [255, 200, 100, 255, 120, 30, 80, 255, ...] where (255, 200, 100, 255) would be the pixel on 1st row and 1st column and (120, 30, 80, 255) the pixel on 1st row and 2nd column and so on.</param>.
/// <param name="bFlipVertically">By default we use the OpenGL convention : the first row should be the bottom of the image. You can set bFlipVertically to false if your first row is at the top of the image.</param>
void AsPNG(const char* filepath, int width, int height, void* data, bool bFlipVertically = true);

} // namespace Cool::ExportImage