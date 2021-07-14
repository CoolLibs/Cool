#include "OrbitalState_Idle.h"
#include "OrbitalState_Rotation.h"
#include "OrbitalState_Translation.h"

namespace Cool {

using OrbitalState = std::variant<
    OrbitalState_Idle,
    OrbitalState_Rotation,
    OrbitalState_Translation>;

// TEST : Every OrbitalState implements the IOrbitalState interface
static_assert(std::is_base_of_v<IOrbitalState, OrbitalState_Idle>);
static_assert(std::is_base_of_v<IOrbitalState, OrbitalState_Rotation>);
static_assert(std::is_base_of_v<IOrbitalState, OrbitalState_Translation>);

} // namespace Cool