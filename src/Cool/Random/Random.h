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
	static float range_0to1();

	/**
	 * @brief
	 * 
	 * @return A random float between -1 and 1 
	 */
	static float range_minus1to1();

	/**
	 * @brief
	 * 
	 * @return A random long int (between std::numeric_limits<long int>::min() and std::numeric_limits<long int>::max())
	 */
	static long int long_int();

private:
	static std::random_device _true_random_generator;
	static std::default_random_engine _generator;
	static std::uniform_real_distribution<float> _distribution_0to1;
	static std::uniform_real_distribution<float> distribution_minus1to1;
	static std::uniform_int_distribution<long int> distribution_long_int;

	Random() = delete;
	~Random() = delete;
};

} // namespace Cool