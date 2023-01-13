#pragma once
#include <Cool/Core/BuildMode.h>

namespace Cool {

struct WindowConfig {
    const char* title;                                                            // The title that will be displayed in the title bar of the window
    int         initial_width               = 1280;                               // Initial width of the window
    int         initial_height              = 720;                                // Initial height of the window
    bool        cap_framerate_on_startup_if = true;                               // Whether the framerate should be capped or not (this can later be changed at runtime through window.cap_framerate_if())
    bool        maximize_on_startup_if      = is_building_as(BuildMode::Release); // Whether the window will be maximized when the application starts.
    bool        hide_on_startup_if          = false;                              // Whether the window will be hidden when the application starts.
};

} // namespace Cool
