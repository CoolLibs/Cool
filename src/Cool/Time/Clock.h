#pragma once

namespace Cool {

class Clock {
public:
    Clock()          = default;
    virtual ~Clock() = default;

    [[nodiscard]] virtual auto delta_time() const -> float = 0;
    [[nodiscard]] virtual auto time() const -> float       = 0;

    virtual void set_time(float new_time, bool force_delta_time_to_ignore_the_change = false) = 0;
    virtual void update()                                                                     = 0;

    virtual void       play();
    virtual void       pause();
    void               set_playing(bool wants_to_play);
    void               toggle_play_pause();
    [[nodiscard]] auto is_playing() const -> bool { return _is_playing; }

    auto is_being_forced_to_not_respect_realtime() const -> bool { return _is_being_changed_in_GUI; }
    void set_is_being_changed_in_GUI(bool state) { _is_being_changed_in_GUI = state; }

private:
    bool _is_playing{true};
    bool _is_being_changed_in_GUI{false};
};

} // namespace Cool