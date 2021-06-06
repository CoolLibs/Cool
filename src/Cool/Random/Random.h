#pragma once

#include <random>

namespace Cool {

class Random {
public:
	/**
	 * @brief 
	 * 
	 * @return A random float between 0 and 1 
	 */
	static float get0to1();

	/**
	 * @brief
	 * 
	 * @return A random float between -1 and 1 
	 */
	static float getMinus1to1();

	/**
	 * @brief
	 * 
	 * @return A random long int
	 */
	static long int getAnyLongInt();

private:
	static std::random_device trueRandomGenerator;
	static std::default_random_engine generator;
	static std::uniform_real_distribution<float> distribution_0to1;
	static std::uniform_real_distribution<float> distribution_minus1to1;
	static std::uniform_int_distribution<long int> distribution_long_int;

	Random() = delete;
	~Random() = delete;
};

} // namespace Cool