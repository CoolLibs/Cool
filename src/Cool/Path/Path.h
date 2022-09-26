#pragma once

#include <filesystem>

namespace Cool {

class Path {
public:
    /// Root directory where the program was started from. Use this to describe your ressources by an absolute path, especially if you play at runtime with the directories and your relative paths get messed up
    static std::filesystem::path const& root();

    /// Directory where "Cool/res" is located, or where the Cool resources have been copied. This path will always be valid to refer to "Cool/res"
    static std::filesystem::path const& cool_res();

    /// Initializes root. This must be called once, at the start of your application
    static void initialize_root(std::filesystem::path);

    /// Initializes cool_res. This must be called once, at the start of your application
    static void initialize_cool_res(std::filesystem::path);

private:
    static std::filesystem::path _root;
    static std::filesystem::path _cool_res;
#if DEBUG
    static bool _root_is_initialized;
    static bool _cool_res_is_initialized;
#endif
};

} // namespace Cool
