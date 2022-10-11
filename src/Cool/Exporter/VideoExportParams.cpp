#include "VideoExportParams.h"
#include <Cool/ImGui/ImGuiExtras.h>

namespace Cool {

void VideoExportParams::imgui()
{
    ImGui::InputFloat("FPS", &fps);

    ImGui::PushItemWidth(50);
    ImGui::Text("From");
    ImGui::SameLine();
    ImGui::PushID(13540);
    ImGui::DragFloat("", &beginning);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::Text("To");
    ImGui::SameLine();
    ImGui::PushID(14540);
    ImGui::DragFloat("", &end);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::Text("seconds");
    ImGui::PopItemWidth();

    ImGui::InputInt("Numbering offset", &frame_numbering_offset, 0);
    ImGui::SameLine();
    ImGuiExtras::help_marker("Name of the first image that will be exported for this video sequence. This doesn't change the actual rendered content at all, just the names of the image files.");
}

} // namespace Cool