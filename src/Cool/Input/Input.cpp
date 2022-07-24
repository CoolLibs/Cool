#include "Input.h"
#include <GLFW/glfw3.h>

namespace Cool {

bool UserInput::matches_char(const char* character, int key)
{
    if (key == -1)
    {
        return false;
    }
    const char* name = glfwGetKeyName(key, 0);
    return name && strcmp(name, character) == 0;
}

} // namespace Cool