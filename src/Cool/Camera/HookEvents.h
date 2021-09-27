#pragma once
#include <Cool/Events/MouveEventDispatcher.h>
#include "ViewController_Orbital.h"

namespace Cool {

void hook_events(MouveEventDispatcher<ViewCoordinates>& dispatcher, ViewController_Orbital& controller, Camera& camera);

} // namespace Cool
