#pragma once

namespace Cool {

class Clock {
public:
    Clock()          = default;
    virtual ~Clock() = default;

    [[nodiscard]] virtual auto delta_time() const -> float = 0;
    [[nodiscard]] virtual auto time() const -> float       = 0;

    virtual void set_time(float new_time) = 0;
    virtual void update()                 = 0;

    virtual void       play();
    virtual void       pause();
    void               set_playing(bool wants_to_play);
    void               toggle_play_pause();
    [[nodiscard]] auto is_playing() const -> bool { return _is_playing; }

private:
    bool _is_playing = true;
};

} // namespace Cool