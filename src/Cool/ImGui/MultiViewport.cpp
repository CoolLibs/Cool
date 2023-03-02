#include "MultiViewport.h"
#include <Cool/AppManager/should_we_use_a_separate_thread_for_update.h>
#include "Cool/UserSettings/UserSettings.h"

namespace Cool {

void enable_imgui_multiviewport()
{
#if !defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD) // Platform windows freeze if we are not rendering on the main thread (TODO(JF) : need to investigate that bug ; it is probably coming directly from ImGui)
    if (user_settings().hack_reset_imgui_context_each_frame) // Multiviewport doesn't work while we use this hack
        return;

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
}

void disable_imgui_multiviewport()
{
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
}

} // namespace Cool