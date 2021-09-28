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

bool Input::MatchesChar(const char* character, int key)
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

glm::vec2 Input::MouseInScreenCoordinates()
{
    // double x, y;
    // glfwGetCursorPos(s_mainWindow, &x, &y);
    // return glm::vec2(x, y) - glm::vec2(RenderState::InAppRenderArea().topLeft());
    return {0, 0};
}

glm::vec2 Input::MouseInNormalizedRatioSpace()
{
    // glm::vec2 pos = MouseInScreenCoordinates();
    // pos /= RenderState::InAppRenderArea().height();
    // pos.y = 1.0f - pos.y;
    // pos *= 2.0f;
    // pos -= glm::vec2(RenderState::InAppRenderArea().aspectRatio(), 1.0f);
    // return pos;
    return {0, 0};
}

glm::vec2 Input::ToNormalizedRatioSpace(double xPos, double yPos)
{
    // glm::vec2 pos = glm::vec2(xPos, yPos) - glm::vec2(RenderState::InAppRenderArea().topLeft());
    // pos /= RenderState::InAppRenderArea().height();
    // pos.y = 1.0f - pos.y;
    // pos *= 2.0f;
    // pos -= glm::vec2(RenderState::InAppRenderArea().aspectRatio(), 1.0f);
    // return pos;
    return {0, 0};
}

} // namespace Cool