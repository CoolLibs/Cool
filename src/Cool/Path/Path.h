#pragma once
#include <filesystem>
#include "Cool/Path/PathsConfig.h"

namespace Cool {

class Path {
public:
    /// Root directory where the program was started from. Use this to describe your ressources by an absolute path, especially if you play at runtime with the directories and your relative paths get messed up
    [[nodiscard]] static auto root() -> std::filesystem::path const&;

    /// Directory where "Cool/res" is located, or where the Cool resources have been copied. This path will always be valid to refer to "Cool/res"
    [[nodiscard]] static auto cool_res() -> std::filesystem::path const&;

    /// Path to the folder where all the user data are stored (imgui.ini, etc.).
    [[nodiscard]] static auto user_data() -> std::filesystem::path const&;

    /// Path to the default texture used by TextureInfo.
    [[nodiscard]] static auto default_texture() -> std::filesystem::path const&;

    /// Initializes the paths. This must be called once, at the start of your application.
    template<typename PathsConfigT>
    static void initialize()
    {
        _paths_config = std::make_unique<PathsConfigT>();
    }

private:
    static std::unique_ptr<PathsConfig> _paths_config;
};

} // namespace Cool
