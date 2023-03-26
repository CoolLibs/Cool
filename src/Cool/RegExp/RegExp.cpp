#include "RegExp.h"
#include <regex>

namespace Cool::RegExp {

auto file_path_to_include(std::string const& line) -> std::optional<std::filesystem::path>
{
    static auto const regexp = std::regex{R"(#include *\"(.*)\")"}; // Needs to be static because constructing a regexp is a bit expensive.

    std::smatch matches;
    if (!std::regex_search(line, matches, regexp))
        return std::nullopt;

    return std::filesystem::path{std::string{matches[1]}};
}

auto hash_define(std::string const& line) -> std::optional<HashDefine>
{
    // Matches "#define", then any whitespaces (\s+), then one word (\w+), then any whitespaces (\s+), then everything else (.*).
    static auto const regexp = std::regex{R"(#define\s+(\w+)\s+(.*))"}; // Needs to be static because constructing a regexp is a bit expensive.

    std::smatch matches;
    if (!std::regex_search(line, matches, regexp))
        return std::nullopt;

    return HashDefine{.name = std::string{matches[1]}, .value = std::string{matches[2]}};
}

} // namespace Cool::RegExp

#if COOL_ENABLE_TESTS
#include <doctest/doctest.h>
TEST_CASE("[Cool::RegExp] #include")
{
    // Given
    const auto text =
        "some random text;\n"
        "#include   \"filename.ext\"ranbom_bits\n"
        "more randomness\n";
    // When
    const auto result = Cool::RegExp::file_path_to_include(text);
    // Then
    CHECK(result.has_value());
    CHECK(*result == "filename.ext");
}
#endif