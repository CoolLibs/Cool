#include "../Time.h"

#include "Clock_Realtime.h"
#include "Clock_FixedTimestep.h"
#include <Cool/ImGui/ImGui.h>
#include <Cool/App/RenderState.h>
#include <Cool/Icons/Icons.h>

namespace Cool {

std::unique_ptr<Clock> Time::_clock = nullptr;

void Time::initialize() {
    _clock = std::make_unique<Clock_Realtime>();
}

void Time::update() {
    _clock->update();
}

float Time::delta_time() {
    return _clock->delta_time();
}

float Time::time() {
    return _clock->time();
}

void Time::set_time(float new_time) {
    _clock->set_time(new_time);
}

void Time::set_elapse_mode_as_realtime() {
    float t = time();
    _clock = std::make_unique<Clock_Realtime>();
    _clock->set_time(t);
}

void Time::set_elapse_mode_as_fixed_timestep(float fps) {
    _clock = std::make_unique<Clock_FixedTimestep>(fps, time());
}

void Time::imgui_timeline() {
    if (!RenderState::IsExporting()) {
        if (ImGui::ButtonWithIcon(_clock->is_playing() ? Icons::Pause() : Icons::Play())) {
            _clock->toggle_play_pause();
        }
        ImGui::SameLine();
        float t = time();
        if (ImGui::DragFloat("seconds", &t, 0.5f, 0.f, 0.f, "%.2f")) {
            set_time(t);
        }
    }
    else {
        ImGui::Text("%.2f seconds", time());
    }
}

} // namespace Cool