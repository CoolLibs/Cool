#pragma once

namespace Cool {

/**
 * @brief Configuration options that control the behaviour of the AppManager
 *
 */
struct AppManagerConfig {
    // Decides whether ImGui is allowed to react to key events (NB : doesn't affect text inputs, only things like moving in a list with up / down arrows).
    bool dispatch_keyboard_events_to_imgui = true;
};

} // namespace Cool
