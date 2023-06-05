#include "icon_fmt.h"
#include "Cool/UserSettings/UserSettings.h"

namespace Cool {

auto icon_fmt(std::string_view text, std::string_view icon, bool only_when_extra_icons) -> std::string
{
    if (only_when_extra_icons && !user_settings().extra_icons)
        return std::string{text};
    return fmt::format(" {} {}", icon, text);
}

} // namespace Cool