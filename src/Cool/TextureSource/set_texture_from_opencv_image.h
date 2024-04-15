#pragma once
#include "Cool/Gpu/Texture.h"
#include "opencv2/opencv.hpp"

namespace Cool {

void set_texture_from_opencv_image(std::optional<Texture>& texture, cv::Mat const& image);

}