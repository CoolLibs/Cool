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
}

} // namespace Cool