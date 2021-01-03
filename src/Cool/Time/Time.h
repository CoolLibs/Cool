#pragma once

namespace Cool {

class Clock;

/// <summary>
/// The current local time of the app. Be default it elapses at the same speed as real time, but you can set it to use a fixed timestep (usefull when exporting images)
/// </summary>
class Time {
public:
	/// <summary>
	/// Must be called before the beginning of the app
	/// </summary>
	static void Initialize();
	/// <summary>
	/// Must be called on every frame.
	/// </summary>
	static void Update();
#if defined(__COOL_IMGUI) && defined(__COOL_ICONS)
	/// <summary>
	/// Shows the current time on a timeline, with a few controls.
	/// </summary>
	static void ImGuiTimeline();
#endif
	/// <summary>
	/// Returns the current time in seconds.
	/// </summary>
	static float time();
	/// <summary>
	/// Returns the timestep between two frames, in seconds.
	/// </summary>
	static float deltaTime();
	/// <summary>
	/// Sets the current time.
	/// </summary>
	/// <param name="t">New time in seconds</param>
	static void setTime(float t);

	/// <summary>
	/// Makes the time elapse as fast as real time.
	/// </summary>
	static void SetAsRealtime();
	/// <summary>
	/// Makes the time elapse at a constant rate between two frames, independantly of how much real time has actually elapsed.
	/// </summary>
	/// <param name="fps">The framerate at which the time should elapse.</param>
	static void SetAsFixedTimestep(float fps = 60.f);

private:
	static std::unique_ptr<Clock> m_clock;
};

} // namespace Cool