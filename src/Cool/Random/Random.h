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
	 * @return A random uint64_t (between 0 and std::numeric_limits<uint64_t>::max())
	 */
    static uint64_t uint64();

private:
    static std::random_device                      _true_random_generator;
    static std::default_random_engine              _generator;
    static std::uniform_real_distribution<float>   _distribution_0to1;
    static std::uniform_real_distribution<float>   _distribution_minus1to1;
    static std::uniform_int_distribution<uint64_t> _distribution_uint64;

    Random()  = delete;
    ~Random() = delete;
};

} // namespace Cool