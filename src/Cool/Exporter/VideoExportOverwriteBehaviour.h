#pragma once

namespace Cool {

enum class VideoExportOverwriteBehaviour {
    AskBeforeCreatingNewFolder,
    AskBeforeOverwritingPreviousFrames,
    AlwaysCreateNewFolder,
    AlwaysOverwritePreviousFrames,
};

inline auto imgui_widget(VideoExportOverwriteBehaviour& behaviour) -> bool
{
    return ImGui::Combo("When export folder is not empty", reinterpret_cast<int*>(&behaviour), "Ask before creating a new folder\0Ask before overwriting the previous frames\0Always create a new folder\0Always overwrite the previous frames\0\0"); // NOLINT(*reinterpret-cast)
}

} // namespace Cool