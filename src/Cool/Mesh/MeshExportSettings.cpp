#include "MeshExportSettings.hpp"
#include "Cool/File/File.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

void MeshExportSettings::set_file_name_to_an_unused_name()
{
    // TODO(Mesh)
    // Cool::File::find_available_name(const std::filesystem::path &folder_path, const std::filesystem::path &file_name, const std::filesystem::path &extension)
}

auto MeshExportSettings::imgui() -> bool
{
    bool b = false;
    b |= ImGuiExtras::file_and_folder("Path", &path, NfdFileFilter::Mesh);
    b |= imgui_combo("Format", format);
    if (std::filesystem::exists(path))
        Cool::ImGuiExtras::warning_text(Cool::icon_fmt("This file already exists. The file will be overwritten.", ICOMOON_WARNING).c_str());
    return b;
}

} // namespace Cool