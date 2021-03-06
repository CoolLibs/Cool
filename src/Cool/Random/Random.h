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

    /**
     * @return Any random int
     */
    static int integer();

    /// Returns a random int between min (included) and max (included)
    static int integer(int min, int max);

    /// Returns a random size_t between min (included) and max (included)
    static size_t size_type(size_t min, size_t max);

private:
    static std::random_device                      _true_random_generator;
    static std::default_random_engine              _generator;
    static std::uniform_real_distribution<float>   _distribution_0to1;
    static std::uniform_real_distribution<float>   _distribution_minus1to1;
    static std::uniform_int_distribution<uint64_t> _distribution_uint64;
    static std::uniform_int_distribution<int>      _distribution_integer;

    Random()  = delete;
    ~Random() = delete;
};

} // namespace Cool