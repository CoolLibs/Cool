#pragma once

#include <GLFW/glfw3.h>

namespace Cool {

class Input {
public:
    /**
     * @brief Checks if a glfw key-code corresponds to a given character in the current keyboard layout
     * 
     * @param character A const char* containing the single character that you want to check against
     * @param key The key code that you want to check
     */
    static bool matches_char(const char* character, int key);

    /// <summary>
    /// Returns whether the specified key is currently pressed. /!\ This is about physical keys, independent of the keyboard layout !
    /// </summary>
    /// <param name="glfwKeyCode">The key to check ; something like GLFW_KEY_A, GLFW_KEY_B etc.</param>
    static bool KeyIsDown(int glfwKeyCode);

    static bool CtrlOrCmdIsDown();
    static bool ShiftIsDown();
    static bool AltIsDown();

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