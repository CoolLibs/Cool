#pragma once

namespace Cool {

struct WindowCreationParams {
    const char* title;                // The title that will be displayed in the title bar of the window
    int         width         = 1280; // Initial width of the window
    int         height        = 720;  // Initial height of the window
    bool        cap_framerate = true; // Whether the framerate should be capped or not (this can later be changed at runtime through window.cap_framerate())
};

} // namespace Cool
