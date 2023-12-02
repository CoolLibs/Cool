#pragma once

namespace Cool::internal {

class IAudioInput {
public:
    IAudioInput()                                               = default;
    IAudioInput(IAudioInput const&)                             = delete;
    auto operator=(IAudioInput const&) noexcept -> IAudioInput& = delete;
    IAudioInput(IAudioInput&&)                                  = default;
    auto operator=(IAudioInput&&) noexcept -> IAudioInput&      = default;
    virtual ~IAudioInput()                                      = default;

    /// Will call the `callback` once for each of the `frames_count` audio frames around the current moment in time.
    /// If there are several channels in a frame, the `callback` will only be called once for all of these channels, and will be given the average of the samples.
    virtual void for_each_audio_frame(int64_t frames_count, std::function<void(float)> const& callback) const = 0;
    /// Returns the number of frames contained in a second of the audio.
    [[nodiscard]] virtual auto sample_rate() const -> float = 0;
    /// Returns true iff the input is currently receiving some audio data (e.g. the music player is playing, or a microphone is beeing used, etc.)
    [[nodiscard]] virtual auto is_playing() const -> bool = 0;

    /// Will be called every frame. Various inputs can do whatever the need there.
    virtual void update() = 0;
    /// Will be called when the input needs to start feeding data.
    virtual void start() = 0;
    /// Will be called when the input needs to stop feeding data.
    virtual void stop() = 0;

    virtual auto               imgui(bool needs_to_highlight_error) -> bool = 0;
    [[nodiscard]] virtual auto does_need_to_highlight_error() const -> bool = 0;
};

} // namespace Cool::internal