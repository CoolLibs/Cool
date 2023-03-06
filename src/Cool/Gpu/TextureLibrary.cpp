#include "TextureLibrary.h"
#include <chrono>
#include <filesystem>
#include <optional>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/FileWatcher/FileWatcher.h"
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureInfo.h"
#include "Cool/Gpu/TextureLibrary.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "fmt/chrono.h"
#include "imgui.h"

namespace Cool {

static auto gen_dummy_texture() -> Texture
{
    return Texture{img::Size{1, 1}, 3, std::array<uint8_t, 3>{255, 0, 255}.data()};
}

static constexpr auto time_to_live = 5min;

static auto time_to_live_has_expired(std::chrono::steady_clock::time_point date_of_last_request) -> bool
{
    return std::chrono::steady_clock::now() - date_of_last_request > time_to_live;
}

auto TextureLibrary::get(std::filesystem::path const& path) -> Texture const&
{
    auto const it = _textures.find(path);
    // If this path is not known to the library, add it and try again.
    if (it == _textures.end())
    {
        _textures[path] = {
            .file_watcher         = FileWatcher{path, FileWatcher_NoCallbacks()},
            .date_of_last_request = clock::now(),
        };
        reload_texture(path);
        return get(path);
    }

    // We have a new request
    it->second.date_of_last_request = clock::now();

    auto const& maybe_tex = it->second.texture;
    if (!maybe_tex && !it->second.error_message) // Texture ise nullopt simply because it hasn't been used in a while. Reload the texture.
        reload_texture(path);

    if (!maybe_tex)
    {
        static auto const dummy_texture = gen_dummy_texture();
        return dummy_texture;
    }
    return *maybe_tex;
}

void TextureLibrary::reload_texture(std::filesystem::path const& path)
{
    try
    {
        _textures[path].error_message = std::nullopt;
        _textures[path].texture = std::nullopt;
        _textures[path].texture = Texture{img::load(path)};
        if (DebugOptions::log_when_creating_textures())
            Log::ToUser::info("TextureLibrary", fmt::format("Generated texture from {}", path));
    }
    catch (std::exception const& e)
    {
        _textures[path].error_message = e.what();
    }
}

auto TextureLibrary::update() -> bool
{
    bool       has_changed = false;
    auto const reload_tex  = [&](std::filesystem::path const& path) {
        reload_texture(path);
        has_changed = true;
    };
    for (auto& kv : _textures)
    {
        kv.second.file_watcher.update({.on_file_changed = reload_tex, .on_path_invalid = reload_tex});
        if (time_to_live_has_expired(kv.second.date_of_last_request))
            kv.second.texture = std::nullopt;
    }

    return has_changed;
}

auto TextureLibrary::error_from(std::filesystem::path const& path) -> std::optional<std::string>
{
    return _textures[path].error_message;
}

void TextureLibrary::imgui_debug_view() const
{
    static constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame
                                             | ImGuiTableFlags_Resizable
                                             | ImGuiTableFlags_BordersOuter
                                             | ImGuiTableFlags_BordersV
                                             | ImGuiTableFlags_BordersH
                                             | ImGuiTableFlags_ContextMenuInBody;

    if (ImGui::BeginTable("tex_library", 3, flags))
    {
        for (auto const& kv : _textures)
        {
            ImGui::TableNextRow();
            {
                ImGui::TableSetColumnIndex(0);
                if (kv.second.texture)
                    ImGuiExtras::image_framed(kv.second.texture->imgui_texture_id(), {100.f * kv.second.texture->aspect_ratio(), 100.f}, 2.f);
                else
                    ImGui::TextUnformatted("INVALID");
            }
            {
                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(kv.first.string().c_str());
            }
            {
                ImGui::TableSetColumnIndex(2);
                auto const time_since_last_use = std::chrono::steady_clock::now() - kv.second.date_of_last_request;
                if (time_since_last_use < time_to_live)
                {
                    auto const duration = std::chrono::duration<float>{time_to_live - time_since_last_use};
                    ImGuiExtras::time_formated_hms(duration.count());
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