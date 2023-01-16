#pragma once
#include <Cool/Input/MouseEventDispatcher.h>
#include "ViewController_Orbital.h"

namespace Cool::CameraU {

void hook_events(MouseEventDispatcher<ViewCoordinates>& dispatcher, ViewController_Orbital& controller, Camera& camera);

} // namespace Cool::CameraU
