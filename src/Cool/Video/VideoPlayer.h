#pragma once
#include <filesystem>
#include <opencv2/videoio.hpp>
#include "Cool/Gpu/Texture.h"

namespace Cool {

struct CurrentImage {
    Texture texture;
    int     frame{};
};

class VideoPlayer {
public:
    auto path() const -> std::filesystem::path const& { return _path; }
    void set_path(std::filesystem::path path)
    {
        _path = std::move(path);
        create_capture();
    }
    auto get_texture(float time_in_seconds) -> Texture const*;

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

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("File Path", _path)
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(
            _path
        );
        create_capture();
    }
};

} // namespace Cool