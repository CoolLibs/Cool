#pragma once

namespace Cool {

template<typename T>
class Averager {
public:
	Averager() {
		clear();
	}
	~Averager() = default;

	void push(T value) {
		N++;
		m_average = (m_average * (N-1) +  value) / N;
	}

	operator T() {
		return m_average;
	}

	void clear() {
		m_average = 0;
		N = 0;
	}

private:
	T m_average;
	unsigned int N;
};

} // namespace Cool