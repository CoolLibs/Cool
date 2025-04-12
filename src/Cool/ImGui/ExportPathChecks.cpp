#include "ExportPathChecks.hpp"
#include "Cool/File/File.h"

namespace Cool {

auto export_path_existence_check() -> ExportPathCheck
{
    return [](std::filesystem::path const& file_path) {
        return File::exists(file_path)
                   ? fmt::format("\"{}\" already exists. Are you sure you want to overwrite it?", Cool::File::file_name(file_path))
                   : ""s;
    };
}

auto ExportPathChecks::compute_all_warnings(std::filesystem::path const& file_path) const -> std::vector<std::string>
{
    auto warnings = std::vector<std::string>{};
    for (auto const& check : warnings_checks)
    {
        auto const maybe_warning = check(file_path);
        if (!maybe_warning.empty())
            warnings.push_back(maybe_warning);
    }
    return warnings;
}

auto ExportPathChecks::is_valid(std::filesystem::path const& file_path) const -> bool
{
    return std::all_of(
        warnings_checks.begin(), warnings_checks.end(),
        [&](ExportPathCheck const& warning) { return warning(file_path).empty(); }
    );
}

} // namespace Cool