#pragma once
#include <cstdint>
#include "Cool/Dependencies/SharedVariable.h"
#include "Cool/Variables/Variable_TimeSpeed.h"

namespace Cool {

class Clock {
public:
    Clock()          = default;
    virtual ~Clock() = default;

    /// Current time
    [[nodiscard]] auto time_in_seconds() const -> float;
    /// Time elapsed since the last call to update() (or 0 if update() has not been called yet).
    /// Guaranteed to be constant in-between two update() calls (only changes when update is called, so that you can properly integrate with your delta time and not miss any change).
    [[nodiscard]] auto delta_time_in_seconds() const -> float;

    /// Must be called once per frame
    void update();
    void set_time(float new_time_in_seconds, bool force_delta_time_to_ignore_the_change = false);
    auto time_speed() const -> SharedVariable<TimeSpeed> const& { return _time_speed; }
    auto time_speed() -> SharedVariable<TimeSpeed>& { return _time_speed; }

    void               play();
    void               pause();
    void               set_playing(bool wants_to_play);
    void               toggle_play_pause();
    [[nodiscard]] auto is_playing() const -> bool { return _is_playing; }

    auto is_being_forced_to_not_respect_realtime() const -> bool { return _is_being_changed_in_GUI; }
    void set_is_being_changed_in_GUI(bool state) { _is_being_changed_in_GUI = state; }

private:
    /// Called once every frame when the clock is playing
    virtual auto update_and_get_delta_time_in_nanoseconds() -> int64_t = 0;
    /// Called when the clock goes from paused to playing
    virtual void on_play(){};

private:
    int64_t                   _time_in_nanoseconds{0};
    int64_t                   _delta_time_in_nanoseconds{0};
    int64_t                   _extra_delta_time_in_nanoseconds{0};
    bool                      _is_playing{true};
    bool                      _is_being_changed_in_GUI{false};
    SharedVariable<TimeSpeed> _time_speed{Variable<TimeSpeed>{VariableData<TimeSpeed>{
                                              .name     = "Time Speed",
                                              .value    = {1.f},
                                              .metadata = {
                                                  .bounds = {.has_min_bound = true},
                                              },
                                          }},
                                          {} /*Empty DirtyFlag, we don't need to warn anyone when this changes*/};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Time (in nanoseconds)", _time_in_nanoseconds),
            cereal::make_nvp("Speed", _time_speed),
            cereal::make_nvp("Is playing", _is_playing)
        );
    }
};

} // namespace Cool