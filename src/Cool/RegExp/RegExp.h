#pragma once

namespace Cool {
/// Represents a `#define name value` macro
struct HashDefine {
    std::string name;
    std::string value;
};
} // namespace Cool

namespace Cool::RegExp {

auto file_path_to_include(std::string const& line) -> std::optional<std::filesystem::path>;
auto hash_define(std::string const& line) -> std::optional<HashDefine>;

} // namespace Cool::RegExp