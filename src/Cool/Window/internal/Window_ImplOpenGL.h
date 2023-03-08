#pragma once
#if defined(COOL_OPENGL)
#include "GLFW/glfw3.h"

namespace Cool {

class Window_ImplOpenGL {
public:
    void               cap_framerate_if(bool should_cap, GLFWwindow* window);
    [[nodiscard]] auto framerate_is_capped() const -> bool { return _framerate_is_capped; }

private:
    bool _framerate_is_capped{true};
};

} // namespace Cool

#endif