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
	static float time();
	static float deltaTime();
	static void setTime(float t);

	static void SetAsRealtime();
	static void SetAsFixedTimestep(float fps);

private:
	static std::unique_ptr<Clock> m_clock;
};

} // namespace Cool