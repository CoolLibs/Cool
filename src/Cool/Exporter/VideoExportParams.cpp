#include "VideoExportParams.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <imgui.h>
#include <cstdint>
#include "internal/origin_of_frames.h"

namespace Cool {

static auto time_to_frames(Time time, double fps) -> int64_t
{
    return static_cast<int64_t>(std::ceil(time.as_seconds_double() * fps)) + origin_of_frames;
}

static auto frames_to_time(int64_t frame, double fps) -> Time
{
    return Time::seconds(static_cast<double>(frame - origin_of_frames) / fps);
}

void VideoExportParams::imgui()
{
    ImGui::Text("Framerate");
    ImGui::SameLine();
    ImGui::InputScalar("##FPS", ImGuiDataType_Double, &fps, nullptr, nullptr, "%.2f fps");

    // TODO(Time) Do two tabs (or a button-icon to switch the mode?), one for seconds and other for frames. To reduce confusion and indicate users that they need to input as one or the other.
    ImGui::PushItemWidth(0.4f * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Fromto").x));
    ImGui::Text("From");
    ImGui::SameLine();
    ImGuiExtras::drag_time("##beginning", &beginning);
    ImGui::SameLine();
    ImGui::Text("to");
    ImGui::SameLine();
    ImGuiExtras::drag_time("##end", &end);
    ImGui::PopItemWidth();

    ImGui::PushItemWidth(75);
    ImGui::Text("From");
    ImGui::SameLine();
    {
        int64_t beginning_frame = time_to_frames(beginning, fps);
        if (ImGui::InputScalar("##beginning2", ImGuiDataType_S64, &beginning_frame))
            beginning = frames_to_time(beginning_frame, fps);
    }
    ImGui::SameLine();
    ImGui::Text("to");
    ImGui::SameLine();
    {
        int64_t end_frame = time_to_frames(end, fps);
        if (ImGui::InputScalar("##end2", ImGuiDataType_S64, &end_frame))
            end = frames_to_time(end_frame, fps);
    }
    ImGui::SameLine();
    ImGui::Text("frames");
    ImGui::PopItemWidth();
}

} // namespace Cool