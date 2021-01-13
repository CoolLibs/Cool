#pragma once

#include <random>

namespace Cool {

class Random {
public:
	/// <summary>
	/// Returns a random float between 0 and 1
	/// </summary>
	static float get0to1();
	/// <summary>
	/// Returns a random float between -1 and 1
	/// </summary>
	static float getMinus1to1();

private:
	static std::random_device trueRandomGenerator;
	static std::default_random_engine generator;
	static std::uniform_real_distribution<float> distribution_0to1;
	static std::uniform_real_distribution<float> distribution_minus1to1;

	Random() = delete;
	~Random() = delete;
};

} // namespace Cool