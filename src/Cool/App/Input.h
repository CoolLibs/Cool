#pragma once

#include <GLFW/glfw3.h>

namespace Cool {

class Input {
public:
    /// <summary>
    /// Checks if a glfw key code corresponds to a given character in the current keyboard layout
    /// </summary>
    /// <param name="character">A const char* containing the single character that you want to check against</param>
    /// <param name="key">The key code that you want to check</param>
    static bool MatchesChar(const char* character, int key);

    /// <summary>
    /// Returns whether the specified key is currently pressed. /!\ This is about physical keys, independent of the keyboard layout !
    /// </summary>
    /// <param name="glfwKeyCode">The key to check ; something like GLFW_KEY_A, GLFW_KEY_B etc.</param>
    static bool KeyIsDown(int glfwKeyCode);

    static bool CtrlOrCmdIsDown();
    static bool ShiftIsDown();
    static bool AltIsDown();

    /// Relative to the viewing area, not the whole window !
    /// (0, 0) is in the top-left corner
    /// Screen coordinates are kind of like pixels, but they are independent of the DPI of the user's screen, so this is what you should use most of the time.
    static glm::vec2 MouseInScreenCoordinates();

    /// Relative to the viewing area, not the whole window !
    /// (0, 0) is in the middle
    /// x is in the range [-aspectRatio, +aspectRatio] Axis pointing right
    /// y is in the range [-1, 1] Axis poiting up
    static glm::vec2 MouseInNormalizedRatioSpace();

    /// Converts coordinates returned by glfw to the proper space.
    /// Relative to the viewing area, not the whole window !
    /// (0, 0) is in the middle
    /// x is in the range [-aspectRatio, +aspectRatio] Axis pointing right
    /// y is in the range [-1, 1] Axis poiting up
    static glm::vec2 ToNormalizedRatioSpace(double xPos, double yPos);

private:
    friend class AppManager;
    static void Initialize(GLFWwindow* mainWindow);

private:
    static GLFWwindow* s_mainWindow;
#ifdef DEBUG
    static bool s_initialized;
#endif
};

} // namespace Cool