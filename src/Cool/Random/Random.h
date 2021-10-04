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
	 * @return A random int64_t (between std::numeric_limits<int64_t>::min() and std::numeric_limits<int64_t>::max())
	 */
    static int64_t long_int();

private:
    static std::random_device                     _true_random_generator;
    static std::default_random_engine             _generator;
    static std::uniform_real_distribution<float>  _distribution_0to1;
    static std::uniform_real_distribution<float>  distribution_minus1to1;
    static std::uniform_int_distribution<int64_t> distribution_long_int;

    Random()  = delete;
    ~Random() = delete;
};

} // namespace Cool