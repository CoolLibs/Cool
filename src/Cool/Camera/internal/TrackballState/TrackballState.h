#include "TrackballState_Idle.h"
#include "TrackballState_Rotation.h"
#include "TrackballState_Translation.h"

namespace Cool {

using TrackballState = std::variant<
      TrackballState_Idle,
      TrackballState_Rotation,
      TrackballState_Translation
>;

} // namespace Cool