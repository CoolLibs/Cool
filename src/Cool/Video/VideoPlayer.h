#pragma once
#include <easy_ffmpeg/src/VideoDecoder.hpp>
#include <filesystem>
#include <memory>
#include "Cool/Gpu/Texture.h"
#include "easy_ffmpeg/easy_ffmpeg.hpp"

namespace Cool {

enum class VideoPlayerLoopMode {
    None,
    Loop,
    Hold,
    // TODO(Video) Mode Bounce (like mirror repeat for textures). But requires us to support playing videos backward
};

struct VideoPlayerSettings {
    VideoPlayerLoopMode loop_mode{VideoPlayerLoopMode::Loop};
    float               playback_speed{1.f};
    float               start_time{0.f};

    friend auto operator==(VideoPlayerSettings const&, VideoPlayerSettings const&) -> bool = default;
    auto        imgui_widget() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Loop mode", loop_mode),
            cereal::make_nvp("Playback speed", playback_speed),
            cereal::make_nvp("Start time", start_time)
        );
    }
};

namespace internal {
class CaptureState {
public:
    static auto create(std::filesystem::path const& path) -> tl::expected<internal::CaptureState, std::string>;

    [[nodiscard]] auto get_texture(float time_in_seconds, VideoPlayerSettings const& settings, std::filesystem::path const& path) -> Texture const&;

    [[nodiscard]] auto detailed_video_info() const -> std::string const& { return _capture->detailed_info(); }

private:
    explicit CaptureState(std::filesystem::path const& path)
        : _capture{std::make_unique<ffmpeg::VideoDecoder>(path)}
    {}

private:
    std::unique_ptr<ffmpeg::VideoDecoder> _capture;
    std::optional<Texture>                _texture{};
    double                                _frames_per_second{};
    int                                   _frames_count{};
    int                                   _frame_in_texture{-1};
    int                                   _next_frame_in_capture{0};
};
} // namespace internal

class VideoPlayer {
public:
    [[nodiscard]] auto path() const -> std::filesystem::path const& { return _path; }
    void               set_path(std::filesystem::path path);
    [[nodiscard]] auto settings() const -> VideoPlayerSettings const& { return _settings; }
    [[nodiscard]] auto settings() -> VideoPlayerSettings& { return _settings; }

    [[nodiscard]] auto get_texture(float time_in_seconds) -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string> const& { return _error_message; }

    auto imgui_widget() -> bool;

    friend auto operator==(VideoPlayer const& a, VideoPlayer const& b) -> bool
    {
        return a._settings == b._settings
               && a._path == b._path;
    }

private:
    void create_capture();

private:
    std::filesystem::path _path{};
    VideoPlayerSettings   _settings{};

    std::optional<internal::CaptureState> _capture_state{};
    std::optional<std::string>            _error_message{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("File Path", _path),
            cereal::make_nvp("Settings", _settings)
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(_path, _settings);
        create_capture();
    }
};

} // namespace Cool