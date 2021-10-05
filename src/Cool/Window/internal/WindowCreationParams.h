#pragma once
#include <Cool/Core/BuildMode.h>

namespace Cool {

struct WindowCreationParams {
    const char* title;                                       // The title that will be displayed in the title bar of the window
    int         width                  = 1280;               // Initial width of the window
    int         height                 = 720;                // Initial height of the window
    BuildMode   cap_framerate_if       = BuildMode::Any;     // Whether the framerate should be capped or not (this can later be changed at runtime through window.cap_framerate())
    BuildMode   maximize_on_startup_if = BuildMode::Release; // Whether the window will be maximized when the application starts.
    BuildMode   hide_on_startup_if     = BuildMode::None;    // Whether the window will be hidden when the application starts.
};

} // namespace Cool
