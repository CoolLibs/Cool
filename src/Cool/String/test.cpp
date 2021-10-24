#include "String.h"

TEST_CASE("[Cool::String] replace()")
{
    using namespace std::string_literals;
    // Given
    const std::string                                      text         = "ssdgd ${hello}${world}ssd{c}v"s;
    const std::vector<std::pair<std::string, std::string>> replacements = {std::make_pair("hello"s, "1"s),
                                                                           std::make_pair("world"s, "2"s)};
    // When
    const auto result = Cool::String::replace({text, replacements});
    // Then
    CHECK(result == "ssdgd 12ssd{c}v");
    const auto replace_next_res = Cool::String::replace_next({text, replacements}, 0);
    CHECK(replace_next_res.second.has_value());
    CHECK(replace_next_res.first == "ssdgd 1${world}ssd{c}v");
    CHECK(replace_next_res.second.value() == 7);
    CHECK(Cool::String::find_replacement("hello", replacements).value() == "1");
    CHECK(Cool::String::find_replacement("hell", replacements) == std::nullopt);
}

TEST_CASE("[Cool::String] replace_at()")
{
    CHECK(Cool::String::replace_at(3, 5, "0123456789", "abcde") == "012abcde56789");
}