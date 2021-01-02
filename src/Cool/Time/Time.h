#pragma once

namespace Cool {

class Clock;

class Time {
public:
	static void Initialize();
	static void Update();
#if defined(__COOL_IMGUI) && defined(__COOL_ICONS)
	static void ImGuiTimeline();
#endif
	// In seconds
	static float time();
	static float deltaTime();
	// In seconds
	static void setTime(float t);

private:
	static std::unique_ptr<Clock> m_clock;
};

} // namespace Cool