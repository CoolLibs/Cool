#include "Clock_Realtime.h"

namespace Cool {

Clock_Realtime::Clock_Realtime()
    : m_initialTime(std::chrono::steady_clock::now()), m_lastTime(m_initialTime), m_currentTime(m_lastTime)
{}

float Clock_Realtime::deltaTime() const {
    if (Clock::isPlaying()) {
        std::chrono::duration<float> dt = m_currentTime - m_lastTime;
        return dt.count();
    }
    else
        return 0.f;
}

float Clock_Realtime::time() const {
    std::chrono::duration<float> delta = m_currentTime - m_initialTime;
	return delta.count() - m_offsetWithSTDTime;
}

void Clock_Realtime::setTime(float newTime) {
    m_offsetWithSTDTime += time() - newTime;
}

void Clock_Realtime::update() {
    if (Clock::isPlaying()) {
        m_lastTime = m_currentTime;
        m_currentTime = std::chrono::steady_clock::now();
    }
}

void Clock_Realtime::play() {
    Clock::play();
    std::chrono::duration<float> delta = std::chrono::steady_clock::now() - m_timeWhenPaused;
    m_offsetWithSTDTime += delta.count();
    update();
}

void Clock_Realtime::pause() {
    Clock::pause();
    m_timeWhenPaused = std::chrono::steady_clock::now();
}

} // namespace Cool