#pragma once

namespace Cool {

struct WindowCreationParams {
    /**
	 * @param name The name that will be displayed in the title bar of the window
	 * @param width Initial width of the window
	 * @param height Initial height of the window
     * @param cap_framerate Whether the framerate should be capped or not (this can later be changed at runtime through window.cap_framerate())
     */
    WindowCreationParams(const char* name, int width = 1280, int height = 720, bool cap_framerate = true)
        : name{name}, width{width}, height{height}, cap_framerate{cap_framerate}
    {
    }

    const char* name;
    int         width;
    int         height;
    bool        cap_framerate;
};

} // namespace Cool
