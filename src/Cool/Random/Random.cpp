#include "Random.h"

namespace Cool {

std::random_device                      Random::_true_random_generator;
std::default_random_engine              Random::_generator(_true_random_generator());
std::uniform_real_distribution<float>   Random::_distribution_0to1(0.0f, 1.0f);
std::uniform_real_distribution<float>   Random::distribution_minus1to1(-1.0f, 1.0f);
std::uniform_int_distribution<long int> Random::distribution_long_int(std::numeric_limits<long int>::min(), std::numeric_limits<long int>::max());

float Random::range_0to1()
{
    return _distribution_0to1(_generator);
}

float Random::range_minus1to1()
{
    return distribution_minus1to1(_generator);
}

long int Random::long_int()
{
    return distribution_long_int(_generator);
}

} // namespace Cool