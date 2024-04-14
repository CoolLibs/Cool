#include "TextureSource_FromVideoFile.h"
#include "Cool/Gpu/TextureLibrary_VideoFile.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"
#include "Cool/Video/hack_get_global_time_in_seconds.h"
#include "TextureLibrary_VideoFile.h"

namespace Cool {

auto VideoPlayerSettings::imgui_widget() -> bool
{
    bool b = false;
    b |= ImGuiExtras::toggle("Does loop", &do_loop);
    b |= ImGui::DragFloat("Playback speed", &playback_speed, 1.f, 0.f, FLT_MAX, "x%.2f", ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
    b |= ImGui::DragFloat("Time offset", &time_offset, 1.f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_NoRoundToFormat);
    return b;
}

auto TextureSource_FromVideoFile::imgui_widget() -> bool
{
    bool b    = false;
    auto path = _player->path();
    if (ImGuiExtras::file_and_folder("File", &path, NfdFileFilter::Video))
    {
        b = true;
        _player->set_path(path);
    }
    b |= settings.imgui_widget();
    return b;
}

[[nodiscard]] auto TextureSource_FromVideoFile::get_texture() const -> Texture const*
{
    return _player->get_texture(hack_get_global_time_in_seconds());
}

auto TextureSource_FromVideoFile::get_error() const -> std::optional<std::string>
{
    return {}; // Cool::TextureLibrary_FromWebcam::instance().error_from(webcam_name);
}

} // namespace Cool