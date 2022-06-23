#pragma once

namespace Cool {

class Path {
public:
    /// Root directory where the program was started from. Use this to describe your ressources by an absolute path, especially if you play at runtime with the directories and your relative paths get messed up
    static const std::string& root();

    /// Directory where Cool/res is located, or where the Cool resources have been copied. This path will always be valid to refer to Cool/res
    static const std::string& cool_res();

    /// Initializes root. This must be called once, at the start of your application
    static void initialize_root(std::string_view path);

    /// Initializes cool_res. This must be called once, at the start of your application
    static void initialize_cool_res(std::string_view path);

private:
    static std::string _root;
    static std::string _cool_res;
#if DEBUG
    static bool _root_is_initialized;
    static bool _cool_res_is_initialized;
#endif
};

} // namespace Cool
