#include "WindowFactoryU.h"

namespace Cool::WindowFactoryU {

void apply_config(Window& window, WindowCreationParams config)
{
    window.cap_framerate(is_building_as(config.cap_framerate_if));
}

} // namespace Cool::WindowFactoryU
