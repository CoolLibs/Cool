#include "TextureLibrary_VideoFile.h"
#include <chrono>
#include <filesystem>
#include <optional>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Time/time_formatted_hms.h"
#include "Cool/Video/hack_get_global_time_in_seconds.h"
#include "imgui.h"

namespace Cool {

static constexpr auto time_to_live = 1min;

static auto time_to_live_has_expired(std::chrono::steady_clock::time_point date_of_last_request) -> bool
{
    return std::chrono::steady_clock::now() - date_of_last_request > time_to_live;
}

auto TextureLibrary_VideoFile::get(VideoRequest const& request) -> Texture const*
{
    auto const key = Key{request.id, request.video_descriptor.path};
    auto       it  = _videos.find(key);
    if (it == _videos.end())
    {
        it = _videos.insert({key, Cool::TextureLibrary_VideoFile::Data{
                                      .player{Cool::VideoPlayer{request.video_descriptor}},
                                      .date_of_last_request = clock::now(),
                                  }})
                 .first;
    }
    else
    {
        it->second.player.settings() = request.video_descriptor.settings; // Make sure to update the settings
    }

    // We have a new request
    it->second.date_of_last_request = clock::now();

    return it->second.player.get_texture(hack_get_global_time_in_seconds());
}

void TextureLibrary_VideoFile::update()
{
    std::erase_if(_videos, [&](auto const& kv) {
        return time_to_live_has_expired(kv.second.date_of_last_request);
    });
}

auto TextureLibrary_VideoFile::error_from(VideoRequest const& request) -> std::optional<std::string>
{
    auto it = _videos.find({request.id, request.video_descriptor.path});
    if (it == _videos.end())
        return std::nullopt;
    return it->second.player.get_error();
}

auto TextureLibrary_VideoFile::detailed_video_info(std::filesystem::path const& path) const -> std::string const*
{
    for (auto const& kv : _videos)
    {
        if (kv.first.path != path)
            continue;

        std::string const* res = kv.second.player.detailed_video_info();
        if (res == nullptr)
            continue;

        return res;
    }
    return nullptr;
}

void TextureLibrary_VideoFile::imgui_debug_view() const
{
    static constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame
                                             | ImGuiTableFlags_Resizable
                                             | ImGuiTableFlags_BordersOuter
                                             | ImGuiTableFlags_BordersV
                                             | ImGuiTableFlags_BordersH
                                             | ImGuiTableFlags_ContextMenuInBody;

    if (ImGui::BeginTable("vid_library", 3, flags))
    {
        for (auto const& kv : _videos)
        {
            ImGui::TableNextRow();
            {
                ImGui::TableSetColumnIndex(0);
                auto const* maybe_tex = kv.second.player.get_current_texture();
                if (maybe_tex)
                    ImGuiExtras::image_framed(maybe_tex->imgui_texture_id(), {100.f * maybe_tex->aspect_ratio(), 100.f}, {.frame_thickness = 2.f, .flip_y = true});
                else
                    ImGui::TextUnformatted("INVALID");
            }
            {
                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(kv.first.path.string().c_str());
            }
            {
                ImGui::TableSetColumnIndex(2);
                auto const time_since_last_use = std::chrono::steady_clock::now() - kv.second.date_of_last_request;
                if (time_since_last_use < time_to_live)
                {
                    auto const duration = std::chrono::duration<float>{time_to_live - time_since_last_use};
                    ImGui::TextUnformatted(time_formatted_hms(duration.count()).c_str());
                }
                else
                {
                    ImGui::TextUnformatted(fmt::format("Expired (Not used for {})", time_to_live).c_str());
                }
            }
        }
        ImGui::EndTable();
    }
}

} // namespace Cool