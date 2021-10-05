#pragma once
#include <Cool/Input/MouveEventDispatcher.h>
#include "ViewController_Orbital.h"

namespace Cool::CameraU {

void hook_events(MouveEventDispatcher<ViewCoordinates>& dispatcher, ViewController_Orbital& controller, Camera& camera);

} // namespace Cool::CameraU
