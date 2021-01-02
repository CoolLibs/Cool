#include "../Time.h"

#include "Clock_Realtime.h"
#if defined(__COOL_IMGUI) && defined(__COOL_ICONS)
#include <Cool/Icons/Icons.h>
#include <Cool/ImGui/ImGui.h>
#endif

namespace Cool {

std::unique_ptr<Clock> Time::m_clock = nullptr;

void Time::Initialize() {
    m_clock = std::make_unique<Clock_Realtime>();
}

void Time::Update() {
    m_clock->update();
}

float Time::deltaTime() {
    return m_clock->deltaTime();
}

float Time::time() {
    return m_clock->time();
}

void Time::setTime(float t) {
    m_clock->setTime(t);
}

#if defined(__COOL_IMGUI) && defined(__COOL_ICONS)
void Time::ImGuiTimeline() {
    if (CoolImGui::ButtonWithIcon(m_clock->isPlaying() ? Icons::Pause() : Icons::Play())) {
        m_clock->togglePlayPause();
    }
    ImGui::SameLine();
    float t = time();
    if (ImGui::DragFloat("seconds", &t, 0.5f, 0.f, 0.f, "%.2f")) {
        setTime(t);
    }
}
#endif

} // namespace Cool