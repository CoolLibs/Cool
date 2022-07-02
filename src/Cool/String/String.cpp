#include "String.h"

namespace Cool::String {

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

std::optional<std::pair<size_t, size_t>> find_next_word(std::string_view text, size_t offset, std::string_view delimiters)
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

std::vector<std::string> split_into_words(std::string_view text, std::string_view delimiters)
{
    std::vector<std::string> res;
    auto                     word_pos = find_next_word(text, 0, delimiters);
    while (word_pos.has_value())
    {
        res.emplace_back(text.substr(word_pos->first, word_pos->second - word_pos->first));
        word_pos = find_next_word(text, word_pos->second, delimiters);
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
    const auto pos = Cool::String::find_next_word(text, starting_pos, delimiters);
    if (pos)
    {
        return std::string{text.substr(pos->first, pos->second - pos->first)};
    }
    else
    {
        return "";
    }
}

} // namespace Cool::String