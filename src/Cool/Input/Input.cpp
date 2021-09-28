#include "Input.h"

namespace Cool {

GLFWwindow* Input::s_mainWindow;
#ifdef DEBUG
bool Input::s_initialized = false;
#endif

void Input::Initialize(GLFWwindow* mainWindow)
{
    s_mainWindow = mainWindow;
#ifdef DEBUG
    if (s_initialized)
        Log::warn("[Input::Initialize] You are calling Initialize() twice.");
    s_initialized = true;
#endif
}

bool Input::matches_char(const char* character, int key)
{
    if (key == -1)
        return false;
    const char* name = glfwGetKeyName(key, 0);
    return name && strcmp(name, character) == 0;
}

bool Input::KeyIsDown(int glfwKeyCode)
{
    return glfwGetKey(s_mainWindow, glfwKeyCode) == GLFW_PRESS;
}

bool Input::CtrlOrCmdIsDown()
{
    return KeyIsDown(GLFW_KEY_LEFT_CONTROL) || KeyIsDown(GLFW_KEY_RIGHT_CONTROL);
}

bool Input::ShiftIsDown()
{
    return KeyIsDown(GLFW_KEY_LEFT_SHIFT) || KeyIsDown(GLFW_KEY_RIGHT_SHIFT);
}

bool Input::AltIsDown()
{
    return KeyIsDown(GLFW_KEY_LEFT_ALT) || KeyIsDown(GLFW_KEY_RIGHT_ALT);
}

} // namespace Cool