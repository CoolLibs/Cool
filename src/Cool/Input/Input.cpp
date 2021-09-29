#include "Input.h"
#include <glfw/glfw3.h>

namespace Cool {

bool Input::matches_char(const char* character, int key)
{
    if (key == -1) {
        return false;
    }
    const char* name = glfwGetKeyName(key, 0);
    return name && strcmp(name, character) == 0;
}

} // namespace Cool