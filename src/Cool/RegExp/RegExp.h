#pragma once

#include <regex>

namespace Cool::RegExp {

static const std::regex includes{R"(#include *\"(.*)\")"};

inline auto file_path_to_include(const std::string& text) -> std::optional<std::string>
{
    std::smatch matches;
    if (std::regex_search(text, matches, includes))
    {
        return matches[1];
    }
    else
    {
        return std::nullopt;
    }
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