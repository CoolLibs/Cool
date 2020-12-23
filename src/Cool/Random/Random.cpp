#include "Random.h"

namespace Cool {

std::random_device MyRand::trueRandomGenerator;
std::default_random_engine MyRand::generator(trueRandomGenerator());
std::uniform_real_distribution<float> MyRand::distribution_0to1(0.0f, 1.0f);
std::uniform_real_distribution<float> MyRand::distribution_minus1to1(-1.0f, 1.0f);

float Random::get0to1() {
	return distribution_0to1(generator);
}

float Random::getMinus1to1() {
	return distribution_minus1to1(generator);
}

} // namespace Cool