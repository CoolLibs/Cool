#pragma once

#include <filesystem>

namespace Cool {

class Path {
public:
    /// Root directory where the program was started from. Use this to describe your ressources by an absolute path, especially if you play at runtime with the directories and your relative paths get messed up
    [[nodiscard]] static auto root() -> std::filesystem::path const&;

    /// Directory where "Cool/res" is located, or where the Cool resources have been copied. This path will always be valid to refer to "Cool/res"
    [[nodiscard]] static auto cool_res() -> std::filesystem::path const&;

    /// Path to the default texture used by TextureInfo.
    [[nodiscard]] static auto default_texture() -> std::filesystem::path const&;

    /// Initializes root. This must be called once, at the start of your application
    static void initialize_root(std::filesystem::path);

    /// Initializes cool_res. This must be called once, at the start of your application
    static void initialize_cool_res(std::filesystem::path);

    /// Initializes default_texture. This must be called once, at the start of your application
    static void initialize_default_texture(std::filesystem::path);

private:
    static std::filesystem::path _root;
    static std::filesystem::path _cool_res;
    static std::filesystem::path _default_texture;
#if DEBUG
    static bool _root_is_initialized;
    static bool _cool_res_is_initialized;
    static bool _default_texture_is_initialized;
#endif
};

} // namespace Cool
