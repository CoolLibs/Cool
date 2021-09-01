#include "VideoExportParams.h"

namespace Cool {

void VideoExportParams::imgui()
{
    ImGui::InputFloat("FPS", &_fps);
    ImGui::PushItemWidth(50);
    ImGui::Text("From");
    ImGui::SameLine();
    ImGui::PushID(13540);
    ImGui::DragFloat("", &_sequence_begin_time_in_sec);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::Text("To");
    ImGui::SameLine();
    ImGui::PushID(14540);
    ImGui::DragFloat("", &_sequence_end_time_in_sec);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::Text("seconds");
    ImGui::PopItemWidth();
}

} // namespace Cool