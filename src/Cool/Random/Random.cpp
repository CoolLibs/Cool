#include "Random.h"

namespace Cool {

std::random_device Random::trueRandomGenerator;
std::default_random_engine Random::generator(trueRandomGenerator());
std::uniform_real_distribution<float> Random::distribution_0to1(0.0f, 1.0f);
std::uniform_real_distribution<float> Random::distribution_minus1to1(-1.0f, 1.0f);
std::uniform_int_distribution<long int> Random::distribution_long_int(std::numeric_limits<long int>::min(), std::numeric_limits<long int>::max());

float Random::get0to1() {
	return distribution_0to1(generator);
}

float Random::getMinus1to1() {
	return distribution_minus1to1(generator);
}

long int Random::getAnyLongInt() {
	return distribution_long_int(generator);
}

} // namespace Cool