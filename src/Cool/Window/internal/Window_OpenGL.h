#pragma once
#if defined(__COOL_APP_OPENGL)

#include "Window_Base.h"

namespace Cool {

class Window_OpenGL : public Window_Base {
public:
    inline void makeCurrent() { glfwMakeContextCurrent(_base.glfw()); }
};

} // namespace Cool

#endif