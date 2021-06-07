#pragma once

namespace Cool {

class Clock;

/**
 * @brief The current time of the app. By default it elapses at the same speed as real time, but you can set it to use a fixed timestep (useful when exporting images).
 * 
 */
class Time {
public:
	/**
	 * @brief Must be called before the beginning of the app
	 * 
	 */
	static void Initialize();

	/**
	 * @brief Must be called on every frame.
	 * 
	 */
	static void Update();

	/**
	 * @brief Shows the current time on a timeline, with a few controls.
	 * 
	 */
	static void ImGuiTimeline();

	/**
	 * @brief 
	 * 
	 * @return The current time in seconds. 
	 */
	static float time();

	/**
	 * @brief 
	 * 
	 * @return The time between two frames, in seconds. (More precisely : the time ellapsed between the last two calls of Update)
	 */
	static float deltaTime();

	/**
	 * @brief Sets the current time.
	 * 
	 * @param t New time in seconds
	 */
	static void setTime(float t);

	/**
	 * @brief Makes the time elapse at real time speed. Counteracts SetAsFixedTimestep().
	 * 
	 */
	static void SetAsRealtime();

	/**
	 * @brief Makes the time elapse at a constant rate between two frames, independantly of how much real time has actually elapsed. Counteracts SetAsRealtime().
	 * 
	 * @param fps The framerate at which the time should elapse.
	 */
	static void SetAsFixedTimestep(float fps = 60.f);

private:
	static std::unique_ptr<Clock> m_clock;
};

} // namespace Cool