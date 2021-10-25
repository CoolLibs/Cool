#include "Random.h"

namespace Cool {

std::random_device                      Random::_true_random_generator;
std::default_random_engine              Random::_generator(_true_random_generator());
std::uniform_real_distribution<float>   Random::_distribution_0to1(0.0f, 1.0f);
std::uniform_real_distribution<float>   Random::_distribution_minus1to1(-1.0f, 1.0f);
std::uniform_int_distribution<uint64_t> Random::_distribution_uint64(0, std::numeric_limits<uint64_t>::max());

float Random::range_0to1()
{
    return _distribution_0to1(_generator);
}

float Random::range_minus1to1()
{
    return _distribution_minus1to1(_generator);
}

uint64_t Random::uint64()
{
    return _distribution_uint64(_generator);
}

} // namespace Cool