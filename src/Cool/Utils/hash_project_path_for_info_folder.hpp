#pragma once
#include "Cool/File/File.h"

namespace Cool {

inline auto hash_project_path_for_info_folder(std::filesystem::path const& file_path) -> std::string
{
    //----------------------
    // IMPORTANT: Don't ever change this implementation. Both Coollab and the launcher use it to know in which folder the infos of a project are stored.
    //            Changing it would break compatibility between Coollab and the launcher, and between various versions of the apps.
    //----------------------
    size_t const value{std::hash<std::string>{}(Cool::File::weakly_canonical(file_path).string())};
    return std::to_string(value);
}

} // namespace Cool