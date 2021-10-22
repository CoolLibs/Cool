#pragma once

namespace Cool::String {
/**
 * @brief 
 * 
 * @param str 
 * @return A new string with all characters converted to lower case
 */
std::string to_lower(std::string_view str);

/**
 * @brief 
 * 
 * @param to_find 
 * @param str 
 * @return true iff *str* starts with *to_find*
 */
bool starts_with(std::string_view to_find, std::string_view str);

/**
 * @brief Modifies *str* by replacing all occurences of *from* with *to*
 * 
 * @param str 
 * @param from 
 * @param to 
 */
void replace_all(std::string& str, std::string_view from, std::string_view to);

/**
 * @brief Converts a number to a string. Adds 0s to the left until the size of the string is greater or equal to min_nb_of_characters.
 * 
 * @param val The number to convert to a string.
 * @param min_nb_of_characters The minimum number of characters that will be present in the output string.
 */
template<typename T>
std::string to_string(T val, int min_nb_of_characters = 0)
{
    std::string str = std::to_string(val);
    int         dn  = min_nb_of_characters - static_cast<int>(str.size());
    if (dn > 0) {
        str = std::string(dn, '0') + str;
    }
    return str;
}

struct ReplacementInput {
    std::string                                      text;
    std::vector<std::pair<std::string, std::string>> replacements;
};

std::string replace(const ReplacementInput& in);

size_t replace_next(const ReplacementInput& in, size_t start_pos);

std::optional<std::string> find_replacement(std::string string_to_replace, const std::vector<std::pair<std::string, std::string>>& replacements);

/**
 * @brief Replaces the characters at positions begin (included) till end (excluded) in input_string with new_substring
 */
std::string replace_at(size_t begin, size_t end, const std::string& input_string, const std::string& new_substring);

} // namespace Cool::String

TEST_CASE("[Cool::String] replace()")
{
    // Given
    const std::string                                      text         = "ssdgd ${hello}ssd{c}v";
    const std::vector<std::pair<std::string, std::string>> replacements = {std::make_pair(std::string{"hello"}, std::string{"has_been_replaced"})};
    // When
    const auto result = Cool::String::replace({text, replacements});
    // Then
    // CHECK(result == "ssdgd has_been_replacedssd{c}v");
    CHECK(Cool::String::find_replacement("hello", replacements).value() == "has_been_replaced");
    CHECK(Cool::String::find_replacement("hell", replacements) == std::nullopt);
}

TEST_CASE("[Cool::String] replace_at()")
{
    CHECK(Cool::String::replace_at(3, 5, "0123456789", "abcde") == "012abcde56789");
}