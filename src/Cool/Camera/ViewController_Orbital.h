#pragma once

#include "internal/OrbitalState/OrbitalState.h"

namespace Cool {

class Camera;

class ViewController_Orbital {
public:
    enum class Mode {
        Trackball,
        Turntable,
        AxisFree
    };

public:
    ViewController_Orbital(ViewController_Orbital::Mode mode = ViewController_Orbital::Mode::Turntable);

    bool ImGui();

    void        set_orbit_center(glm::vec3 const& orbit_center, Camera& camera);
    inline void set_distance_to_orbit_center(float distance) { _distance_to_orbit_center = distance; }
    inline void set_state(OrbitalState state) { _state = state; }

    // clang-format off
    inline void on_mouse_move  (Camera& camera, glm::vec2 const& delta) { std::visit([&](auto&& state) { state.on_mouse_move  (*this, camera, delta); }, _state); }
	inline void on_wheel_down  (Camera& camera, int mods)               { std::visit([&](auto&& state) { state.on_wheel_down  (*this, camera, mods);  }, _state); }
	inline void on_wheel_up    (Camera& camera)                         { std::visit([&](auto&& state) { state.on_wheel_up    (*this, camera);        }, _state); }
	inline void on_wheel_scroll(Camera& camera, float dl)               { std::visit([&](auto&& state) { state.on_wheel_scroll(*this, camera, dl);    }, _state); }
    // clang-format on

private:
    friend class OrbitalState_Idle;
    friend class OrbitalState_Rotation;
    friend class OrbitalState_Translation;

    glm::vec3 get_orbit_center(Camera const& camera) const;

private:
    ViewController_Orbital::Mode _mode;
    float                        _translation_speed        = 0.01f;
    float                        _rotation_speed           = 0.01f;
    float                        _distance_to_orbit_center = 5.f;
    OrbitalState                 _state                    = OrbitalState_Idle{};
};

} // namespace Cool