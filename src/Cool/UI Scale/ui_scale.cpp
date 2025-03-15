#include "ui_scale.hpp"
#include "Cool/UserSettings/UserSettings.h"
#include "screen_dpi_scale.hpp"

namespace Cool {

auto ui_scale() -> float
{
    return user_settings().ui_scale * screen_dpi_scale();
}

} // namespace Cool