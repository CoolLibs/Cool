#include "String.h"
#include <charconv>
#include <exception>
#include <glm/detail/qualifier.hpp>

namespace Cool::String {

auto contains(std::string_view text, std::string_view characters) -> bool // TODO remove me and use std::contains when it arrives in C++23
{
    return text.find(characters) != std::string_view::npos;
}

auto to_lower(std::string_view str) -> std::string
{
    std::string res = "";
    std::for_each(str.begin(), str.end(), [&res](char c) {
        res += static_cast<char>(std::tolower(c));
    });
    return res;
}

auto replace_all(std::string& str, std::string_view from, std::string_view to) -> void
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

auto replace_between_delimiters(const ReplacementInput& in) -> std::string
{
    auto next = replace_next(in, 0);
    while (next.second)
    {
        next = replace_next(
            {next.first, in.replacements, in.delimiter_begin, in.delimiter_end},
            *next.second
        );
    }
    return next.first;
}

auto find_replacement(
    const std::string&                                      string_to_replace,
    const std::vector<std::pair<std::string, std::string>>& replacements
) -> std::optional<std::string>
{
    const auto res = std::find_if(
        replacements.begin(),
        replacements.end(),
        [&](const std::pair<std::string, std::string>& pair) {
            return pair.first == string_to_replace;
        }
    );
    if (res == replacements.end())
    {
        return std::nullopt;
    }
    else
    {
        return res->second;
    }
}

auto replace_next(
    const ReplacementInput& in,
    size_t                  start_pos
) -> std::pair<std::string, std::optional<size_t>>
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
            if (replacement)
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

auto replace_at(
    size_t           begin,
    size_t           end,
    std::string_view input_string, std::string_view new_substring
) -> std::string
{
    return std::string(input_string.substr(0, begin)) + std::string(new_substring) + std::string(input_string.substr(end, input_string.length() - end));
}

auto find_matching_pair(
    std::string_view text,
    char             opening,
    char             closing
) -> std::optional<std::pair<size_t, size_t>>
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

auto find_next_word_position(
    std::string_view text,
    size_t           offset,
    std::string_view delimiters
) -> std::optional<std::pair<size_t, size_t>>
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

auto split_into_words(
    std::string_view text,
    std::string_view delimiters
) -> std::vector<std::string>
{
    std::vector<std::string> res;
    auto                     word_pos = find_next_word_position(text, 0, delimiters);
    while (word_pos)
    {
        res.emplace_back(text.substr(word_pos->first, word_pos->second - word_pos->first));
        word_pos = find_next_word_position(text, word_pos->second, delimiters);
    }
    return res;
}

auto remove_whitespaces(std::string_view text) -> std::string
{
    auto res = std::string{text};
    replace_all(res, " ", "");
    replace_all(res, "\n", "");
    replace_all(res, "\t", "");
    replace_all(res, "\r", "");
    return res;
}

auto next_word(
    std::string_view text,
    size_t           starting_pos,
    std::string_view delimiters
) -> std::optional<std::string>
{
    auto position = find_next_word_position(text, starting_pos, delimiters);
    if (!position)
    {
        return std::nullopt;
    }
    else
    {
        return std::string{text.substr(position->first, position->second - position->first)};
    }
}

auto next_block(
    std::string_view text,
    size_t           ending_key_pos
) -> std::optional<std::string>
{
    const auto start_position_of_block = text.find("(");
    if (start_position_of_block != std::string_view::npos)
    {
        return next_word(text, start_position_of_block + 1, ")");
    }
    else
    {
        return next_word(
            text,
            ending_key_pos
        );
    }
}

auto find_word_following(
    std::string_view text,
    std::string_view key
) -> std::optional<std::string>
{
    const auto start_position_of_key = text.find(key);
    if (start_position_of_key == std::string_view::npos)
    {
        return std::nullopt;
    }
    else
    {
        return next_block(
            text,
            start_position_of_key + key.length()
        );
    }
}

template<typename T>
static auto value_from_string_impl_scalar(std::string_view str) -> std::optional<T>
{
    if constexpr (std::is_same_v<T, int>)
    {
        try
        {
            return std::stoi(std::string(str));
        }
        catch (const std::exception&)
        {
            return std::nullopt;
        }
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        try
        {
            return std::stof(std::string(str));
        }
        catch (const std::exception&)
        {
            return std::nullopt;
        }
    }
    else
    {
        const auto debug_name = std::string{"Type not supported yet: "} + typeid(T).name();
        std::ignore           = debug_name;
        assert(false);
    }

    // Waiting for Xcode to implement std::from_chars
    // T out;
    // const std::from_chars_result result = std::from_chars(str.data(), str.data() + str.size(), out);
    // if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
    // {
    //     return std::nullopt;
    // }
    // return out;
}

template<typename ScalarType, int NbElements>
static auto value_from_string_impl_vec(std::string_view str)
    -> std::optional<glm::vec<NbElements, ScalarType, glm::defaultp>>
{
    std::vector<std::pair<size_t, size_t>> words_positions;

    std::optional<size_t> ptr_in_string = 0; // Offset by 1 to skip the parenthesis

    while (ptr_in_string)
    {
        const auto pos = find_next_word_position(str, *ptr_in_string);
        if (pos)
        {
            words_positions.push_back(*pos);
            ptr_in_string = pos->second;
        }
        else
        {
            ptr_in_string = std::nullopt;
        }
    }

    if (words_positions.size() != NbElements)
    {
        return std::nullopt; // TODO Return an error message instead of std::nullopt (cf std::expected)
    }

    auto ret = glm::vec<NbElements, ScalarType, glm::defaultp>{};
    for (int i = 0; i < NbElements; ++i)
    {
        const auto value = value_from_string_impl_scalar<ScalarType>(
            str.substr(
                words_positions[i].first,
                words_positions[i].second - words_positions[i].first
            )
        );
        if (value)
        {
            ret[i] = *value;
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
    const auto string = to_lower(str);
    if (string == "true")
    {
        return true;
    }
    else if (string == "false")
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
    const auto val = value_from_string_impl_vec<float, 3>(str);
    if (val)
    {
        return Cool::RgbColor{*val};
    }
    else
    {
        return std::nullopt;
    }
}

template<>
auto value_from_string<Cool::Angle>(std::string_view str) -> std::optional<Cool::Angle>
{
    const auto val = value_from_string_impl_scalar<float>(str);
    if (val)
    {
        return Cool::Angle{Cool::Radians{degrees_to_radians(*val)}};
    }
    else
    {
        return std::nullopt;
    }
}

template<>
auto value_from_string<Cool::Direction2D>(std::string_view str) -> std::optional<Cool::Direction2D>
{
    const auto val = value_from_string_impl_scalar<float>(str);
    if (val)
    {
        return Cool::Direction2D{Cool::Angle{Cool::Radians{degrees_to_radians(*val)}}};
    }
    else
    {
        return std::nullopt;
    }
}

template<>
auto value_from_string<Cool::Hue>(std::string_view str) -> std::optional<Cool::Hue>
{
    const auto val = value_from_string_impl_scalar<float>(str);
    if (val)
    {
        return Cool::Hue{*val};
    }
    else
    {
        return std::nullopt;
    }
}

} // namespace Cool::String