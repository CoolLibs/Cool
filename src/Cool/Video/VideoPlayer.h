#pragma once
#include <opencv2/videoio.hpp>
#include "Cool/Gpu/Texture.h"

namespace Cool {

struct CurrentImage {
    Texture texture;
    int     frame{};
};

class VideoPlayer {
public:
    auto set_path(std::filesystem::path path)
    {
        _path = std::move(path);
        create_capture();
    }
    auto get(float time_in_seconds) -> Texture const&;

private:
    void create_capture();

private:
    int                             _next_frame_in_capture{0};
    double                          _frames_per_second{};
    int                             _frames_count{};
    std::filesystem::path           _path{};
    std::optional<cv::VideoCapture> _capture{};
    std::optional<Texture>          _current_texture{};
    int                             _current_frame{};
};

} // namespace Cool