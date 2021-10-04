#pragma once
#include "ModifierKeys.h"

namespace Cool {

struct KeyboardEvent {
    // The physical key (independant of a QWERTY vs AZERTY layout). Something like GLFW_KEY_A, GLFW_KEY_B etc.
    // It might be GLFW_KEY_UNKNOWN if GLFW lacks a key token for it, for example for E-mail and Play keys.
    // If you want to check for a character according to the keyboard layout, use Input::matches_char().
    int key;
    // The scancode is unique for every key, regardless of whether it has a key token.
    // Scancodes are platform-specific but consistent over time, so keys will have different scancodes depending on the platform but they are safe to save to disk.
    // You can query the scancode for any named key on the current platform with glfwGetKeyScancode.
    int scancode;
    // The action is one of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE.
    int action;
    // Modifier keys that where pressed when the even occured.
    ModifierKeys mods;
};

} // namespace Cool
