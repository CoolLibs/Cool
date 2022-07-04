#include "String.h"
#include <charconv>
#include <glm/detail/qualifier.hpp>

namespace Cool::String {

auto contains(std::string_view text, std::string_view characters) -> bool // TODO remove me and use std::contains when it arrives in C++23
{
    return text.find(characters) != std::string_view::npos;
}

std::string to_lower(std::string_view str)
{
    std::string res = "";
    std::for_each(str.begin(), str.end(), [&res](char c) {
        res += static_cast<char>(std::tolower(c));
    });
    return res;
}

void replace_all(std::string& str, std::string_view from, std::string_view to)
{
    if (from.empty())
    {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::string replace_between_delimiters(const ReplacementInput& in)
{
    auto next = replace_next(in, 0);
    while (next.second.has_value())
    {
        next = replace_next({next.first, in.replacements, in.delimiter_begin, in.delimiter_end}, *next.second);
    }
    return next.first;
}

std::optional<std::string> find_replacement(const std::string& string_to_replace, const std::vector<std::pair<std::string, std::string>>& replacements)
{
    const auto res = std::find_if(replacements.begin(), replacements.end(), [&](const std::pair<std::string, std::string>& pair) {
        return pair.first == string_to_replace;
    });
    if (res == replacements.end())
    {
        return std::nullopt;
    }
    else
    {
        return res->second;
    }
}

std::pair<std::string, std::optional<size_t>> replace_next(const ReplacementInput& in, size_t start_pos)
{
    const auto begin = in.text.find(in.delimiter_begin, start_pos);
    if (begin == std::string::npos)
    {
        return std::make_pair(in.text, std::nullopt);
    }
    else
    {
        const auto end = in.text.find(in.delimiter_end, begin);
        if (end == std::string::npos)
        {
            throw std::invalid_argument{"No closing " + std::string{in.delimiter_end} + " found."};
        }
        else
        {
            const auto replacement_begin = begin + in.delimiter_begin.length();
            const auto to_replace        = in.text.substr(replacement_begin, end - replacement_begin);
            const auto replacement       = find_replacement(to_replace, in.replacements);
            if (replacement.has_value())
            {
                return std::make_pair(
                    replace_at(
                        begin,
                        end + in.delimiter_end.length(),
                        in.text, *replacement
                    ),
                    begin + replacement->length()
                );
            }
            else
            {
                throw std::invalid_argument{"'" + to_replace + "' was not found in the list of replacements."};
            }
        }
    }
}

std::string replace_at(size_t begin, size_t end, const std::string& input_string, const std::string& new_substring)
{
    return input_string.substr(0, begin) + new_substring + input_string.substr(end, input_string.length() - end);
}

std::optional<std::pair<size_t, size_t>> find_matching_pair(std::string_view text, char opening, char closing)
{
    const size_t first_open = text.find(opening);
    if (first_open == std::string::npos)
    {
        return std::nullopt;
    }
    size_t counter = 1;
    for (size_t pos = first_open + 1; pos != text.length(); ++pos)
    {
        if (text[pos] == closing)
        {
            counter--;
        }
        else if (text[pos] == opening)
        {
            counter++;
        }
        if (counter == 0)
        {
            return std::make_pair(first_open, pos);
        }
    }
    return std::nullopt;
}

std::optional<std::pair<size_t, size_t>> find_next_word_position(std::string_view text, size_t offset, std::string_view delimiters)
{
    const size_t idx1 = text.find_first_not_of(delimiters, offset);
    if (idx1 == std::string_view::npos)
    {
        return std::nullopt;
    }
    size_t idx2 = text.find_first_of(delimiters, idx1);
    if (idx2 == std::string_view::npos)
    {
        idx2 = text.size();
    }
    return std::make_pair(idx1, idx2);
}

std::optional<std::string> find_next_word(std::string_view text, size_t offset, std::string_view delimiters)
{
    auto position = find_next_word_position(text, offset, delimiters);
    if (position == std::nullopt)
    {
        return std::nullopt;
    }
    else
    {
        return std::string{text.substr(position->first, position->second - position->first)};
    }
}

std::vector<std::string> split_into_words(std::string_view text, std::string_view delimiters)
{
    std::vector<std::string> res;
    auto                     word_pos = find_next_word_position(text, 0, delimiters);
    while (word_pos.has_value())
    {
        res.emplace_back(text.substr(word_pos->first, word_pos->second - word_pos->first));
        word_pos = find_next_word_position(text, word_pos->second, delimiters);
    }
    return res;
}

std::string remove_whitespaces(std::string_view text)
{
    auto res = std::string{text};
    replace_all(res, " ", "");
    replace_all(res, "\n", "");
    replace_all(res, "\t", "");
    replace_all(res, "\r", "");
    return res;
}

auto next_word(std::string_view text, size_t starting_pos, std::string_view delimiters) -> std::string
{
    const auto pos = Cool::String::find_next_word_position(text, starting_pos, delimiters);
    if (pos)
    {
        return std::string{text.substr(pos->first, pos->second - pos->first)};
    }
    else
    {
        return "";
    }
}

auto next_block(
    std::string_view text,
    size_t           ending_key_pos
) -> std::string
{
    auto start_position_of_block = text.find("(");
    if (start_position_of_block == std::string_view::npos)
    {
        return Cool::String::next_word(
            text,
            ending_key_pos
        );
    }
    else
    {
        auto block_bounds = find_next_word_position(text, start_position_of_block, ")");
        return std::string{text.substr(block_bounds->first, block_bounds->second - block_bounds->first)};
    }
}

/// Finds in `text` the value associated with a given `key` (e.g. "default", "min", "max"), as a string.
auto find_value_for_given_key_as_string(
    std::string_view text,
    std::string_view key
) -> std::string
{
    auto start_position_of_key = text.find(key);
    if (start_position_of_key == std::string_view::npos)
    {
        return "";
    }
    else
    {
        return Cool::String::next_block(
            text,
            start_position_of_key + key.length()
        );
    }
}

template<typename T>
static auto value_from_string_impl_scalar(std::string_view str) -> std::optional<T>
{
    T                            out;
    const std::from_chars_result result = std::from_chars(str.data(), str.data() + str.size(), out);
    if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
    {
        return std::nullopt;
    }
    return out;
}

template<typename ScalarType, int NbElements>
static auto value_from_string_impl_vec(std::string_view str) -> std::optional<glm::vec<NbElements, ScalarType, glm::defaultp>>
{
    std::string text(str);

    std::vector<std::pair<size_t, size_t>> out;
    if (find_next_word_position(text, 1) != std::nullopt)
    {
        out.push_back(*find_next_word_position(text, 1));
    }
    else
    {
        return std::nullopt;
    }

    while (find_next_word_position(text, out.back().second) != std::nullopt)
    {
        out.push_back(*find_next_word_position(text, out.back().second));
    }

    if (out.size() != NbElements)
    {
        return std::nullopt; // TODO Return an error message instead of std::nullopt (cf std::expected)
    }

    glm::vec<NbElements, float, glm::defaultp> ret;
    for (int j = 0; j < NbElements; ++j)
    {
        auto value = value_from_string_impl_scalar<ScalarType>(
            text.substr(out[j].first, out[j].second - out[j].first)
        );
        if (value != std::nullopt)
        {
            ret[j] = *value;
        }
        else
        {
            return std::nullopt;
        }
    }
    return ret;
}

template<>
auto value_from_string<int>(std::string_view str) -> std::optional<int>
{
    return value_from_string_impl_scalar<int>(str);
}

template<>
auto value_from_string<float>(std::string_view str) -> std::optional<float>
{
    return value_from_string_impl_scalar<float>(str);
}

template<>
auto value_from_string<bool>(std::string_view str) -> std::optional<bool>
{
    if (str == "true")
    {
        return true;
    }
    else if (str == "false")
    {
        return false;
    }
    else
    {
        return std::nullopt;
    }
}

template<>
auto value_from_string<glm::vec2>(std::string_view str) -> std::optional<glm::vec2>
{
    return value_from_string_impl_vec<float, 2>(str);
}

template<>
auto value_from_string<glm::ivec2>(std::string_view str) -> std::optional<glm::ivec2>
{
    return value_from_string_impl_vec<int, 2>(str);
}

template<>
auto value_from_string<glm::vec3>(std::string_view str) -> std::optional<glm::vec3>
{
    return value_from_string_impl_vec<float, 3>(str);
}

template<>
auto value_from_string<glm::ivec3>(std::string_view str) -> std::optional<glm::ivec3>
{
    return value_from_string_impl_vec<int, 3>(str);
}

template<>
auto value_from_string<glm::vec4>(std::string_view str) -> std::optional<glm::vec4>
{
    return value_from_string_impl_vec<float, 4>(str);
}

template<>
auto value_from_string<glm::ivec4>(std::string_view str) -> std::optional<glm::ivec4>
{
    return value_from_string_impl_vec<int, 4>(str);
}

template<>
auto value_from_string<Cool::RgbColor>(std::string_view str) -> std::optional<Cool::RgbColor>
{
    if (value_from_string_impl_vec<float, 3>(str) != std::nullopt)
    {
        return Cool::RgbColor{*value_from_string_impl_vec<float, 3>(str)};
    }
    else
    {
        return std::nullopt;
    }
}

template<>
auto value_from_string<Cool::Angle>(std::string_view str) -> std::optional<Cool::Angle>
{
    if (value_from_string_impl_scalar<float>(str) != std::nullopt)
    {
        return Cool::Angle{};
    }
    else
    {
        return std::nullopt;
    }
}

template<>
auto value_from_string<Cool::Direction2D>(std::string_view str) -> std::optional<Cool::Direction2D>
{
    if (value_from_string_impl_scalar<float>(str) != std::nullopt)
    {
        return Cool::Direction2D{};
    }
    else
    {
        return std::nullopt;
    }
}

template<>
auto value_from_string<Cool::Hue>(std::string_view str) -> std::optional<Cool::Hue>
{
    if (value_from_string_impl_scalar<float>(str) != std::nullopt)
    {
        return Cool::Hue{};
    }
    else
    {
        return std::nullopt;
    }
}

} // namespace Cool::String