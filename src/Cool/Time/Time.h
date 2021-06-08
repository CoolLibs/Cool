#pragma once

namespace Cool {

class Clock;

/**
 * @brief The current time of the app. By default it elapses at the same speed as real time, but you can set it to use a fixed timesteps (useful when exporting images).
 * 
 */
class Time {
public:
	/**
	 * @brief Must be called before the beginning of the app
	 * 
	 */
	static void initialize();

	/**
	 * @brief Must be called on every frame.
	 * 
	 */
	static void update();

	/**
	 * @brief Shows the current time on a timeline, with a few controls.
	 * 
	 */
	static void imgui_timeline();

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
	static float delta_time();

	/**
	 * @brief Sets the current time.
	 * 
	 * @param new_time New time in seconds
	 */
	static void set_time(float new_time);

	/**
	 * @brief Makes the time elapse at real time speed, independently of how long each frame takes to render. Counteracts set_elapse_mode_as_fixed_timestep().
	 * 
	 */
	static void set_elapse_mode_as_realtime();

	/**
	 * @brief Makes the time elapse at a constant rate between two frames, independently of how much real time has actually elapsed. Counteracts set_elapse_mode_as_realtime().
	 * 
	 * @param fps The framerate at which the time should elapse.
	 */
	static void set_elapse_mode_as_fixed_timestep(float fps = 60.f);

private:
	static std::unique_ptr<Clock> _clock;
};

} // namespace Cool