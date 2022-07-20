#pragma once
#include <Cool/Camera/Camera.h>
#include <Cool/StrongTypes/Angle.h>
#include <Cool/StrongTypes/Direction2D.h>
#include <Cool/StrongTypes/Hue.h>
#include <Cool/StrongTypes/RgbColor.h>

namespace Cool::String {

namespace internal {
static constexpr std::string_view default_word_delimiters{" \n\t\r,;{}[]():/"};
};

auto contains(std::string_view text, std::string_view characters) -> bool; // TODO remove me and use std::contains when it arrives in C++23

/**
 * @brief
 *
 * @param str
 * @return A new string with all characters converted to lower case
 */
auto to_lower(std::string_view str) -> std::string;

/**
 * @brief Modifies *str* by replacing all occurences of *from* with *to*
 *
 * @param str
 * @param from
 * @param to
 */
auto replace_all(std::string& str, std::string_view from, std::string_view to) -> void;

/**
 * @brief Converts a number to a string. Adds 0s to the left until the size of the string is greater or equal to min_nb_of_characters.
 *
 * @param val The number to convert to a string.
 * @param min_nb_of_characters The minimum number of characters that will be present in the output string.
 */
template<typename T>
auto to_string(T val, size_t min_nb_of_characters = 0) -> std::string
{
    std::string str = std::to_string(val);
    size_t      dn  = min_nb_of_characters - str.size();
    if (dn > 0)
    {
        str = std::string(dn, '0') + str;
    }
    return str;
}

struct ReplacementInput {
    const std::string&                                      text;
    const std::vector<std::pair<std::string, std::string>>& replacements;
    std::string_view                                        delimiter_begin = "${";
    std::string_view                                        delimiter_end   = "}";
};

auto replace_between_delimiters(const ReplacementInput& in) -> std::string;

auto replace_next(
    const ReplacementInput& in,
    size_t                  start_pos
) -> std::pair<std::string, std::optional<size_t>>;

auto find_replacement(
    const std::string&                                      string_to_replace,
    const std::vector<std::pair<std::string, std::string>>& replacements
) -> std::optional<std::string>;

/**
 * @brief Replaces the characters at positions begin (included) till end (excluded) in input_string with new_substring
 */
auto replace_at(
    size_t           begin,
    size_t           end,
    std::string_view input_string,
    std::string_view new_substring
) -> std::string;

/**
 * @brief Returns the position of the first *opening* character and the position of the matching *closing* character,
 * or std::nullopt if no such pair was found
 */
auto find_matching_pair(
    std::string_view text,
    char opening = '(', char closing = ')'
) -> std::optional<std::pair<size_t, size_t>>;

/**
 * @brief Returns the indices of the beginning and end of the next word in "text" after position "offset".
 * Words are considered to be separated by one or more characters of "delimiters".
 */
auto find_next_word_position(
    std::string_view text,
    size_t           offset,
    std::string_view delimiters = internal::default_word_delimiters
) -> std::optional<std::pair<size_t, size_t>>;

/**
 * @brief Splits the text and returns the list of words. A new word is created whenever one or more characters of "delimiters" are encountered in the text
 * The words appear in the same order in the list as they do in the text
 */
auto split_into_words(
    std::string_view text,
    std::string_view delimiters = internal::default_word_delimiters
) -> std::vector<std::string>;

auto remove_whitespaces(std::string_view text) -> std::string;

/// Returns the next word after `startingPos`. A word is a block of characters that doesn't contain any of the `delimiters`.
auto next_word(
    std::string_view text,
    size_t           starting_pos,
    std::string_view delimiters = internal::default_word_delimiters
) -> std::optional<std::string>;

/// Obtain the next block text between parentheses, next word if no parentheses.
auto next_block(
    std::string_view text,
    size_t           ending_key_pos
) -> std::optional<std::string>;

/// Finds in `text` the word following a given `key` (e.g. "default", "min", "max").
auto find_word_following(
    std::string_view text,
    std::string_view key
) -> std::optional<std::string>;

/// Finds default value string in `text` following the `key` and returns the correspondant value
template<typename T>
auto find_value_for_given_key(
    std::string_view text,
    std::string_view key
) -> std::optional<T>
{
    const auto default_value = find_word_following(text, key);
    if (default_value)
    {
        return value_from_string<T>(*default_value);
    }
    else
    {
        return std::nullopt;
    }
}

/// Converts the given string into the correspondant value by type
template<typename T>
auto value_from_string(std::string_view) -> std::optional<T>
{
    static_assert(
#include "Cool/Variables/generated/T_is_a_variable_type.inl"
        , "Type not supported yet!"
    );
    /// NB: Use the folliwing code if you need to know the type which is failing:
    // const auto debug_name = std::string{"Type not supported yet: "} + typeid(T).name();
    // std::ignore           = debug_name;
    // assert(false);
    return std::nullopt;
}

template<>
auto value_from_string<int>(std::string_view str) -> std::optional<int>;
template<>
auto value_from_string<float>(std::string_view str) -> std::optional<float>;
template<>
auto value_from_string<bool>(std::string_view str) -> std::optional<bool>;
template<>
auto value_from_string<glm::vec2>(std::string_view str) -> std::optional<glm::vec2>;
template<>
auto value_from_string<glm::ivec2>(std::string_view str) -> std::optional<glm::ivec2>;
template<>
auto value_from_string<glm::vec3>(std::string_view str) -> std::optional<glm::vec3>;
template<>
auto value_from_string<glm::ivec3>(std::string_view str) -> std::optional<glm::ivec3>;
template<>
auto value_from_string<glm::vec4>(std::string_view str) -> std::optional<glm::vec4>;
template<>
auto value_from_string<glm::ivec4>(std::string_view str) -> std::optional<glm::ivec4>;
template<>
auto value_from_string<Cool::RgbColor>(std::string_view str) -> std::optional<Cool::RgbColor>;
template<>
auto value_from_string<Cool::Angle>(std::string_view str) -> std::optional<Cool::Angle>;
template<>
auto value_from_string<Cool::Direction2D>(std::string_view str) -> std::optional<Cool::Direction2D>;
template<>
auto value_from_string<Cool::Hue>(std::string_view str) -> std::optional<Cool::Hue>;

} // namespace Cool::String
