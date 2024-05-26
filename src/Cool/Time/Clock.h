#pragma once
#include <cstdint>
#include "Cool/Dependencies/SharedVariable.h"
#include "Cool/Variables/Variable_TimeSpeed.h"
#include "Time.h"

namespace Cool {

class Clock {
public:
    Clock()          = default;
    virtual ~Clock() = default;

    /// Current time
    [[nodiscard]] auto time() const -> Time;
    /// Time elapsed since the last call to update() (or 0 if update() has not been called yet).
    /// Guaranteed to be constant in-between two update() calls (only changes when update is called, so that you can properly integrate with your delta time and not miss any change).
    [[nodiscard]] auto delta_time() const -> Time;

    /// Must be called once per frame
    void update();
    void set_time(Time new_time, bool force_delta_time_to_ignore_the_change = false);
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
    virtual auto update_and_get_delta_time() -> Time = 0;
    /// Called when the clock goes from paused to playing
    virtual void on_play() {};

private:
    Time                      _time{0s};
    Time                      _delta_time{0s};
    Time                      _extra_delta_time{0s};
    bool                      _is_playing{true};
    bool                      _is_being_changed_in_GUI{false};
    SharedVariable<TimeSpeed> _time_speed{Variable<TimeSpeed>{VariableData<TimeSpeed>{
                                              .name     = "",
                                              .value    = {1.f},
                                              .metadata = {
                                                  .bounds = {
                                                      .min           = 0.,
                                                      .max           = DBL_MAX,
                                                      .has_min_bound = true,
                                                      .has_max_bound = false,
                                                      .drag_speed    = 0.001f,
                                                  },
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
            cereal::make_nvp("Time", _time),
            cereal::make_nvp("Speed", _time_speed),
            cereal::make_nvp("Is playing", _is_playing)
        );
    }
};

} // namespace Cool