#pragma once
#include <Cool/Camera/Camera.h>
#include <Cool/StrongTypes/Angle.h>
#include <Cool/StrongTypes/ColorPalette.h>
#include <Cool/StrongTypes/Direction2D.h>
#include <Cool/StrongTypes/Gradient.h>
#include <Cool/StrongTypes/Hue.h>
#include <Cool/StrongTypes/Point2D.h>
#include <Cool/StrongTypes/PremultipliedRgbaColor.h>
#include <Cool/StrongTypes/RgbColor.h>
#include <Cool/StrongTypes/StraightRgbaColor.h>
#include <vector>

namespace Cool::String {

static constexpr std::string_view default_word_delimiters{" \n\t\r,;{}[]():/+*-=&|^~%!?<>`."};
static constexpr std::string_view default_word_delimiters_except_dot{" \n\t\r,;{}[]():/+*-=&|^~%!?<>`"};

auto contains(std::string_view text, std::string_view characters) -> bool; // TODO remove me and use std::contains when it arrives in C++23

/**
 * @brief
 *
 * @param str
 * @return A new string with all characters converted to lower case
 */
auto to_lower(std::string_view str) -> std::string;

/// Modifies `str` by replacing all occurrences of `from` with `to`.
void replace_all(std::string& str, std::string_view from, std::string_view to);

/// Modifies `str` by replacing all `from` words with `to`.
/// We need to match a whole word: for example we won't replace "hello" in "helloworld" but we will replace it in "hello world".
auto replace_all_words(std::string str, std::string_view from, std::string_view to, std::string_view delimiters = default_word_delimiters) -> std::string;

/**
 * @brief Converts a number to a string. Adds 0s to the left until the size of the string is greater or equal to min_nb_of_characters.
 *
 * @param val The number to convert to a string.
 * @param min_nb_of_characters The minimum number of characters that will be present in the output string.
 */
template<typename T>
auto to_string(T val, int min_nb_of_characters = 0) -> std::string
{
    std::string str = std::to_string(val);
    int         dn  = min_nb_of_characters - static_cast<int>(str.size()); // We must use an int because the result can be negative
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

struct find_matching_pair_params {
    std::string_view text;
    size_t           offset  = 0;
    char             opening = '(';
    char             closing = ')';
};

/// Returns the position of the first `opening` character and the position of the matching `closing` character,
/// or std::nullopt if no such pair was found.
auto find_matching_pair(
    find_matching_pair_params p
) -> std::optional<std::pair<size_t, size_t>>;

/// Returns the position of the last `closing` character before `offset` and the position of the matching `opening` character,
/// or std::nullopt if no such pair was found.
auto rfind_matching_pair(
    find_matching_pair_params p
) -> std::optional<std::pair<size_t, size_t>>;

/// Splits the text and returns the list of words. A new word is created whenever one or more characters of "delimiters" are encountered in the text
/// The words appear in the same order in the list as they do in the text
auto split_into_words(
    std::string_view text,
    std::string_view delimiters = default_word_delimiters
) -> std::vector<std::string>;

auto remove_whitespaces(std::string_view text) -> std::string;

/// Returns true iff all the words in `text` are after a `//`.
auto is_commented_out(std::string_view text) -> bool;

/// /!\ Unlike the usual substr method of std::string, this function does not take a begin and a size, but instead a begin and an end.
/// `begin` is included, `end` is excluded.
auto substring(
    const std::string& text,
    size_t             begin,
    size_t             end
) -> std::string;

/// /!\ The returned string_view is only valid as long as the input string_view is valid!
/// /!\ Unlike the usual substr method of std::string_view, this function does not take a begin and a size, but instead a begin and an end.
/// `begin` is included, `end` is excluded.
auto substring(
    std::string_view text,
    size_t           begin,
    size_t           end
) -> std::string_view;

/// /!\ Unlike the usual substr method of std::string, this function does not take a begin and a size, but instead a begin and an end.
/// `begin` is included, `end` is excluded.
auto substring(
    const std::string&        text,
    std::pair<size_t, size_t> begin_end
) -> std::string;

/// /!\ The returned string_view is only valid as long as the input string_view is valid!
/// /!\ Unlike the usual substr method of std::string_view, this function does not take a begin and a size, but instead a begin and an end.
/// `begin` is included, `end` is excluded.
auto substring(
    std::string_view          text,
    std::pair<size_t, size_t> begin_end
) -> std::string_view;

/// Returns the indices of the beginning and end of the next word in "text" after position "starting_pos".
/// Words are considered to be separated by one or more characters of "delimiters".
auto find_next_word_position(
    std::string_view text,
    size_t           starting_pos,
    std::string_view delimiters = default_word_delimiters
) -> std::optional<std::pair<size_t, size_t>>;

/// Returns the indices of the beginning and end of the previous word in "text" before position "ending_pos".
/// Words are considered to be separated by one or more characters of "delimiters".
auto find_previous_word_position(
    std::string_view text,
    size_t           ending_pos,
    std::string_view delimiters = default_word_delimiters
) -> std::optional<std::pair<size_t, size_t>>;

/// /!\ The returned string_view is only valid as long as the input string_view is valid!
/// Returns the next word after `startingPos`. A word is a block of characters that doesn't contain any of the `delimiters`.
auto next_word(
    std::string_view text,
    size_t           starting_pos = 0,
    std::string_view delimiters   = default_word_delimiters
) -> std::optional<std::string_view>;

/// Returns all the words after `startingPos`. A word is a block of characters that doesn't contain any of the `delimiters`.
auto all_words(
    std::string_view text,
    std::string_view delimiters = default_word_delimiters
) -> std::vector<std::string>;

/// Returns the position of the first block of text in `text` after `offset`.
/// A block is either a single word or a block delimited by parentheses.
auto find_block_position(
    std::string_view text,
    size_t           offset
) -> std::optional<std::pair<size_t, size_t>>;

/// Returns the first block of text in `text` after `offset`.
/// A block is either a single word or a block delimited by parentheses.
/// /!\ The returned string_views are only valid as long as the input string_view is valid!
auto find_block(
    std::string_view text,
    size_t           offset = 0
) -> std::optional<std::string_view>;

/// Returns the block following a given `key` (e.g. "default", "min", "max") inside of `text`.
/// /!\ The returned string_views are only valid as long as the input string_view is valid!
auto find_block_following(
    std::string_view text,
    std::string_view key
) -> std::optional<std::string_view>;

/// Converts the given string into the corresponding value.
template<typename T>
auto value_from_string(std::string_view) -> std::optional<T>; // Undefined template. No generic implementation, only specializations are allowed.

template<>
auto value_from_string<int>(std::string_view) -> std::optional<int>;
template<>
auto value_from_string<float>(std::string_view) -> std::optional<float>;
template<>
auto value_from_string<bool>(std::string_view) -> std::optional<bool>;
template<>
auto value_from_string<glm::vec2>(std::string_view) -> std::optional<glm::vec2>;
template<>
auto value_from_string<glm::ivec2>(std::string_view) -> std::optional<glm::ivec2>;
template<>
auto value_from_string<glm::vec3>(std::string_view) -> std::optional<glm::vec3>;
template<>
auto value_from_string<glm::ivec3>(std::string_view) -> std::optional<glm::ivec3>;
template<>
auto value_from_string<glm::vec4>(std::string_view) -> std::optional<glm::vec4>;
template<>
auto value_from_string<glm::ivec4>(std::string_view) -> std::optional<glm::ivec4>;
template<>
auto value_from_string<Cool::RgbColor>(std::string_view) -> std::optional<Cool::RgbColor>;
template<>
auto value_from_string<Cool::PremultipliedRgbaColor>(std::string_view) -> std::optional<Cool::PremultipliedRgbaColor>;
template<>
auto value_from_string<Cool::StraightRgbaColor>(std::string_view) -> std::optional<Cool::StraightRgbaColor>;
template<>
auto value_from_string<Cool::Angle>(std::string_view) -> std::optional<Cool::Angle>;
template<>
auto value_from_string<Cool::Direction2D>(std::string_view) -> std::optional<Cool::Direction2D>;
template<>
auto value_from_string<Cool::Hue>(std::string_view) -> std::optional<Cool::Hue>;
template<>
auto value_from_string<Cool::Camera>(std::string_view) -> std::optional<Cool::Camera>;
template<>
auto value_from_string<Cool::Gradient>(std::string_view) -> std::optional<Cool::Gradient>;
template<>
auto value_from_string<Cool::Point2D>(std::string_view) -> std::optional<Cool::Point2D>;
template<>
auto value_from_string<Cool::ColorPalette>(std::string_view) -> std::optional<Cool::ColorPalette>;

/// Finds default value string in `text` following the `key` and returns the correspondant value
template<typename T>
auto find_value_for_given_key(
    std::string_view text,
    std::string_view key
) -> std::optional<T>
{
    const auto value_as_string = find_block_following(text, key);
    if (value_as_string)
    {
        return value_from_string<T>(*value_as_string);
    }
    else
    {
        return std::nullopt;
    }
}

/// Returns true iff `word` is present in `text`.
/// A word is delimited by `delimiters`.
/// Note that we only match whole words, so for example "Hello World" is not considered to contain "ell", only "Hello" and "World".
auto contains_word(std::string_view word, std::string_view text, std::string_view delimiters = default_word_delimiters) -> bool;

/// Removes all `//` and `/* */` comments
auto remove_comments(std::string const&) -> std::string;

} // namespace Cool::String
