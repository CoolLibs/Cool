#pragma once
#include "Cool/Gpu/Texture.h"
#include "opencv2/opencv.hpp"
struct AVFrame;

namespace Cool {

void set_texture_from_opencv_image(std::optional<Texture>& texture, cv::Mat const& image);
void set_texture_from_ffmpeg_image(std::optional<Texture>& texture, AVFrame const& image);

} // namespace Cool