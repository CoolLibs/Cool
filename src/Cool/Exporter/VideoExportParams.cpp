#include "VideoExportParams.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <imgui.h>
#include "internal/origin_of_frames.h"

namespace Cool {

static auto seconds_to_frames(float beginning, float fps) -> int
{
    return static_cast<int>(std::ceil(beginning * fps)) + origin_of_frames;
}

static auto frames_to_seconds(int beginning, float fps) -> float
{
    return static_cast<float>(beginning - origin_of_frames) / fps;
}

void VideoExportParams::imgui()
{
    ImGui::Text("Framerate");
    ImGui::SameLine();
    ImGui::InputFloat("##FPS", &fps, 0.f, 0.f, "%.0f fps");

    ImGui::PushItemWidth(75);
    ImGui::Text("From");
    ImGui::SameLine();
    ImGui::InputFloat("##beginning", &beginning);
    ImGui::SameLine();
    ImGui::Text("to");
    ImGui::SameLine();
    ImGui::InputFloat("##end", &end);
    ImGui::SameLine();
    ImGui::Text("seconds");
    ImGui::PopItemWidth();

    ImGui::PushItemWidth(75);
    ImGui::Text("From");
    ImGui::SameLine();
    {
        int beginning_frame = seconds_to_frames(beginning, fps);
        if (ImGui::InputInt("##beginning2", &beginning_frame, 0))
            beginning = frames_to_seconds(beginning_frame, fps);
    }
    ImGui::SameLine();
    ImGui::Text("to");
    ImGui::SameLine();
    {
        int end_frame = seconds_to_frames(end, fps);
        if (ImGui::InputInt("##end2", &end_frame, 0))
            end = frames_to_seconds(end_frame, fps);
    }
    ImGui::SameLine();
    ImGui::Text("frames");
    ImGui::PopItemWidth();
}

} // namespace Cool