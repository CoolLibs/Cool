#pragma once

namespace Cool {

enum class VideoExportOverwriteBehaviour {
    AskBeforeCreatingNewFolder,
    AskBeforeOverwritingPreviousFrames,
    AlwaysCreateNewFolder,
    AlwaysOverwritePreviousFrames,
};

auto imgui_widget(VideoExportOverwriteBehaviour& behaviour) -> bool;

} // namespace Cool