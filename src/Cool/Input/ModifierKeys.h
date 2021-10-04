#pragma once
#include <GLFW/glfw3.h>

namespace Cool {

class ModifierKeys {
public:
    // clang-format off
    [[nodiscard]] bool shift() const { return _mods & GLFW_MOD_SHIFT; }
    [[nodiscard]] bool ctrl()  const { return _mods & GLFW_MOD_CONTROL; }
    [[nodiscard]] bool alt()   const { return _mods & GLFW_MOD_ALT; }
    // clang-format on

    explicit ModifierKeys(int glfw_mods)
        : _mods{glfw_mods}
    {
    }

private:
    int _mods;
};

} // namespace Cool