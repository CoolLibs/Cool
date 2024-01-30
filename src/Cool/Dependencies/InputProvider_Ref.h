#pragma once
#include <functional>
#include "Cool/Audio/AudioManager.h"
#include "Cool/Dependencies/Input.h"
#include "Input.h"

namespace Cool {

class InputProvider_Ref {
public:
    InputProvider_Ref(
        float               render_target_aspect_ratio,
        float               height,
        float               time,
        float               delta_time,
        glm::mat3 const&    camera2D,
        AudioManager const& audio_manager
    )
        : _render_target_aspect_ratio{render_target_aspect_ratio}
        , _height{height}
        , _time{time}
        , _delta_time{delta_time}
        , _camera2D{camera2D}
        , _audio_manager{audio_manager}
    {}

    // TODO(Variable) stop using these types and just give normal names to methods
    float operator()(const Input_AspectRatio&) const
    {
        return _render_target_aspect_ratio;
    }

    float operator()(const Input_Height&) const
    {
        return _height;
    }

    auto operator()(const Input_Time&) const -> float
    {
        return _time;
    }

    auto operator()(const Input_DeltaTime&) const -> float
    {
        return _delta_time;
    }

    auto operator()(const Input_Audio&) const -> AudioManager const&
    {
        return _audio_manager;
    }

    auto operator()(const Input_Camera2D&) const -> glm::mat3
    {
        return _camera2D;
    }

private:
    float                                      _render_target_aspect_ratio;
    float                                      _height;
    float                                      _time;
    float                                      _delta_time;
    glm::mat3                                  _camera2D;
    std::reference_wrapper<AudioManager const> _audio_manager;
};

} // namespace Cool
