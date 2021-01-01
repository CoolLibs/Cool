#pragma once

#include "Clock.h"
#include <chrono>

namespace Cool {

class Clock_Realtime : public Clock {
public:
	Clock_Realtime();
	~Clock_Realtime() = default;

	float deltaTime() const override;
	float time() const override;

	void setTime(float newTime) override;
	void update() override;

	void play() override;
	void pause() override;

private:
	std::chrono::steady_clock::time_point m_initialTime;
	std::chrono::steady_clock::time_point m_lastTime;
	std::chrono::steady_clock::time_point m_currentTime;
	std::chrono::steady_clock::time_point m_timeWhenPaused;
	float m_offsetWithSTDTime = 0.f; // Allows us to setTime as we wish
};

} // namespace Cool