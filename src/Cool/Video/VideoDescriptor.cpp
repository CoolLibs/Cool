#include "VideoDescriptor.h"
#include "Cool/Gpu/TextureLibrary_Video.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

auto VideoPlayerSettings::imgui_widget() -> bool
{
    bool b = false;
    b |= ImGui::Combo("Loop mode", reinterpret_cast<int*>(&loop_mode), "None\0Loop\0Hold\0\0"); // NOLINT(*reinterpret-cast)
    b |= ImGui::DragFloat("Playback speed", &playback_speed, 0.0005f, 0.f, FLT_MAX, "x%.2f", ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
    b |= ImGuiExtras::drag_time("Start time", &start_time);
    return b;
}

auto VideoDescriptor::imgui_widget() -> bool
{
    bool b = false;

    ImGui::SeparatorText("Select File");
    b |= ImGuiExtras::file_and_folder("File", &path, NfdFileFilter::Video);

    std::string const* const detailed_video_info = TextureLibrary_Video::instance().detailed_video_info(path);
    if (detailed_video_info != nullptr)
    {
        ImGui::NewLine();
        ImGuiExtras::help_marker(fmt::format("(Click to copy)\n\n{}", *detailed_video_info).c_str());
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsItemClicked())
            ImGui::SetClipboardText(detailed_video_info->c_str());
    }

    ImGui::NewLine();
    ImGui::SeparatorText("Playback options");
    b |= settings.imgui_widget();

    return b;
}

} // namespace Cool