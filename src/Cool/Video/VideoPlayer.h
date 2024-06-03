#pragma once
#include "Cool/Gpu/Texture.h"
#include "VideoDescriptor.h"
#include "easy_ffmpeg/easy_ffmpeg.hpp"

namespace Cool {

namespace internal {
class CaptureState {
public:
    static auto create(std::filesystem::path const& path) -> tl::expected<internal::CaptureState, std::string>;

    [[nodiscard]] auto get_texture(Time time, VideoPlayerSettings const& settings, std::filesystem::path const& path) -> Texture const*;
    [[nodiscard]] auto get_current_texture() const -> std::optional<Texture> const& { return _texture; }

    [[nodiscard]] auto detailed_video_info() const -> std::string const& { return _capture->detailed_info(); }

private:
    explicit CaptureState(std::filesystem::path const& path);

private:
    std::unique_ptr<ffmpeg::VideoDecoder> _capture;
    std::optional<Texture>                _texture{};
};
} // namespace internal

class VideoPlayer {
public:
    explicit VideoPlayer(VideoDescriptor);

    [[nodiscard]] auto path() const -> std::filesystem::path const& { return _desc.path; }
    [[nodiscard]] auto settings() const -> VideoPlayerSettings const& { return _desc.settings; }
    [[nodiscard]] auto settings() -> VideoPlayerSettings& { return _desc.settings; }

    [[nodiscard]] auto get_texture(Time time) -> Texture const*;
    [[nodiscard]] auto get_current_texture() const -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string> const& { return _error_message; }
    [[nodiscard]] auto detailed_video_info() const -> std::string const*;

private:
    void create_capture();

private:
    VideoDescriptor _desc{};

    std::optional<internal::CaptureState> _capture_state{};
    std::optional<std::string>            _error_message{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Video", _desc)
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(_desc);
        create_capture();
    }
};

} // namespace Cool