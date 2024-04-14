#pragma once
#include <filesystem>
#include <opencv2/videoio.hpp>
#include "Cool/Gpu/Texture.h"

namespace Cool {

namespace internal {
class CaptureState {
public:
    static auto create(std::filesystem::path const& path) -> std::optional<internal::CaptureState>;

    auto get_texture(float time_in_seconds) -> Texture const&;

private:
    cv::VideoCapture       _capture{};
    std::optional<Texture> _texture{};
    double                 _frames_per_second{};
    int                    _frames_count{};
    int                    _frame_in_texture{-1};
    int                    _next_frame_in_capture{0};
};
} // namespace internal

class VideoPlayer {
public:
    auto path() const -> std::filesystem::path const& { return _path; }
    void set_path(std::filesystem::path path);
    auto get_texture(float time_in_seconds) -> Texture const*;

private:
    void create_capture();

private:
    std::filesystem::path                 _path{};
    std::optional<internal::CaptureState> _capture_state{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(cereal::make_nvp("File Path", _path));
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(_path);
        create_capture();
    }
};

} // namespace Cool