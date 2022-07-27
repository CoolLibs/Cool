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
            const auto to_replace        = substring(in.text, replacement_begin, end);
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

auto replace_at(
    size_t           begin,
    size_t           end,
    std::string_view input_string, std::string_view new_substring
) -> std::string
{
    return std::string(substring(input_string, 0, begin)) + std::string(new_substring) + std::string(substring(input_string, end, input_string.length()));
}

auto find_matching_pair(
    find_matching_pair_params p
) -> std::optional<std::pair<size_t, size_t>>
{
    const size_t first_open = p.text.find(p.opening, p.offset);
    if (first_open == std::string::npos)
    {
        return std::nullopt;
    }
    size_t counter = 1;
    for (size_t pos = first_open + 1; pos != p.text.length(); ++pos)
    {
        if (p.text[pos] == p.closing)
        {
            counter--;
        }
        else if (p.text[pos] == p.opening)
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

auto split_into_words(
    std::string_view text,
    std::string_view delimiters
) -> std::vector<std::string>
{
    std::vector<std::string> res;
    auto                     word_pos = find_next_word_position(text, 0, delimiters);
    while (word_pos)
    {
        res.emplace_back(substring(text, *word_pos));
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

auto is_commented_out(std::string_view text) -> bool
{
    const auto comment_pos    = text.find("//");
    const auto first_word_pos = text.find_first_not_of("/ ");
    return comment_pos != std::string_view::npos &&
           comment_pos < first_word_pos;
}

template<typename String>
static auto substring_impl(
    const String& text,
    size_t        begin,
    size_t        end
) -> String
{
    assert(begin != String::npos);
    assert(end != String::npos);
    assert(begin <= end);
    return text.substr(begin, end - begin);
}

auto substring(
    const std::string&        text,
    std::pair<size_t, size_t> begin_end
) -> std::string
{
    return substring_impl(text, begin_end.first, begin_end.second);
}

auto substring(
    std::string_view          text,
    std::pair<size_t, size_t> begin_end
) -> std::string_view
{
    return substring_impl(text, begin_end.first, begin_end.second);
}

auto substring(
    const std::string& text,
    size_t             begin,
    size_t             end
) -> std::string
{
    return substring_impl(text, begin, end);
}

auto substring(
    std::string_view text,
    size_t           begin,
    size_t           end
) -> std::string_view
{
    return substring_impl(text, begin, end);
}

auto find_next_word_position(
    std::string_view text,
    size_t           offset,
    std::string_view delimiters
) -> std::optional<std::pair<size_t, size_t>>
{
    const auto idx1 = text.find_first_not_of(delimiters, offset);
    if (idx1 == std::string_view::npos)
    {
        return std::nullopt;
    }
    auto idx2 = text.find_first_of(delimiters, idx1);
    if (idx2 == std::string_view::npos)
    {
        idx2 = text.size();
    }
    return std::make_pair(idx1, idx2);
}

auto next_word(
    std::string_view text,
    size_t           starting_pos,
    std::string_view delimiters
) -> std::optional<std::string_view>
{
    auto position = find_next_word_position(text, starting_pos, delimiters);
    if (!position)
    {
        return std::nullopt;
    }
    else
    {
        return substring(text, *position);
    }
}

auto find_block_position(
    std::string_view text,
    size_t           offset
) -> std::optional<std::pair<size_t, size_t>>
{
    const auto first_word_position  = find_next_word_position(text, offset);
    auto       parentheses_position = find_matching_pair({.text = text, .offset = offset});
    // if `text` after `offset` doesn't contain neither words and parentheses.
    if (first_word_position == std::nullopt && parentheses_position == std::nullopt)
    {
        return std::nullopt;
    }
    // if `text` after `offset` contains only words but no parentheses.
    if (first_word_position != std::nullopt && parentheses_position == std::nullopt)
    {
        return first_word_position;
    }
    // if `text` after `offset` contains only parentheses with no words into.
    if (first_word_position == std::nullopt && parentheses_position != std::nullopt)
    {
        parentheses_position->second++;
        return parentheses_position;
    }
    // if ending position of the first word is following by a `(`.
    // like: `vec2(1., 0.5)` or `abc(def)` --> return respectively `vec2(1., 0.5)` and `abc(def)`.
    if (first_word_position->second == parentheses_position->first)
    {
        return std::make_pair(first_word_position->first, parentheses_position->second + 1);
    }
    // if the first word is separate from `(` by almost a space or character, return only word.
    // like: vec2 (1., 0.5) or abc (def) --> return respectively `vec2` and `abc`.
    if (first_word_position->first != parentheses_position->first + 1)
    {
        return first_word_position;
    }
    // if `(` is the first character of `text` after `offset`.
    else
    {
        parentheses_position->second++;
        return parentheses_position;
    }
}

auto find_block(
    std::string_view text,
    size_t           offset
) -> std::optional<std::string_view>
{
    const auto block_position = find_block_position(text, offset);
    if (block_position == std::nullopt)
    {
        return std::nullopt;
    }
    else
    {
        return substring(text, *block_position);
    }
}

auto find_block_following(
    std::string_view text,
    std::string_view key
) -> std::optional<std::string_view>
{
    const auto start_position_of_key = text.find(key);
    if (start_position_of_key == std::string_view::npos)
    {
        return std::nullopt;
    }
    else
    {
        return find_block(
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

    std::optional<size_t> ptr_in_string = 0;

    auto opening_parenthesis_position = str.find_first_of("(");
    if (opening_parenthesis_position != std::string_view::npos)
    {
        ptr_in_string = opening_parenthesis_position;
    }

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
            substring(str, words_positions[i])
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