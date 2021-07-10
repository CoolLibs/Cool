#include "TrackballState_Idle.h"
#include "TrackballState_Rotation.h"
#include "TrackballState_Translation.h"

namespace Cool {

using TrackballState = std::variant<
      TrackballState_Idle,
      TrackballState_Rotation,
      TrackballState_Translation
>;

// TEST : Every TrackballState implements the ITrackballState interface
static_assert(std::is_base_of_v<ITrackballState, TrackballState_Idle>);
static_assert(std::is_base_of_v<ITrackballState, TrackballState_Rotation>);
static_assert(std::is_base_of_v<ITrackballState, TrackballState_Translation>);

} // namespace Cool