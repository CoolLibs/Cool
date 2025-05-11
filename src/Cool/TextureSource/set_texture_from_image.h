#pragma once
#include "Cool/WebGPU/Texture.h"

namespace cv {
class Mat;
}
namespace ffmpeg {
struct Frame;
}

namespace Cool {

void set_texture_from_opencv_image(std::optional<Texture>& texture, cv::Mat const& image);
void set_texture_from_ffmpeg_image(std::optional<Texture>& texture, ffmpeg::Frame const& image);

} // namespace Cool