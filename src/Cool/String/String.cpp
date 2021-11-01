#include "String.h"

namespace Cool::String {

std::string to_lower(std::string_view str)
{
    std::string res = "";
    std::for_each(str.begin(), str.end(), [&res](char c) {
        res += std::tolower(c);
    });
    return res;
}

void replace_all(std::string& str, std::string_view from, std::string_view to)
{
    if (from.empty()) {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::string replace(const ReplacementInput& in)
{
    auto next = replace_next(in, 0);
    while (next.second.has_value()) {
        next = replace_next({next.first, in.replacements, in.delimiter_begin, in.delimiter_end}, *next.second);
    }
    return next.first;
}

std::optional<std::string> find_replacement(const std::string& string_to_replace, const std::vector<std::pair<std::string, std::string>>& replacements)
{
    const auto res = std::find_if(replacements.begin(), replacements.end(), [&](const std::pair<std::string, std::string>& pair) {
        return pair.first == string_to_replace;
    });
    if (res == replacements.end()) {
        return std::nullopt;
    }
    else {
        return res->second;
    }
}

std::pair<std::string, std::optional<size_t>> replace_next(const ReplacementInput& in, size_t start_pos)
{
    const auto begin = in.text.find(in.delimiter_begin, start_pos);
    if (begin == std::string::npos) {
        return std::make_pair(in.text, std::nullopt);
    }
    else {
        const auto end = in.text.find(in.delimiter_end, begin);
        if (end == std::string::npos) {
            throw std::invalid_argument{"No closing " + std::string{in.delimiter_end} + " found."};
        }
        else {
            const auto replacement_begin = begin + in.delimiter_begin.length();
            const auto to_replace        = in.text.substr(replacement_begin, end - replacement_begin);
            const auto replacement       = find_replacement(to_replace, in.replacements);
            if (replacement.has_value()) {
                return std::make_pair(replace_at(begin, end + in.delimiter_end.length(), in.text, *replacement),
                                      begin + replacement->length());
            }
            else {
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
    if (first_open == std::string::npos) {
        return std::nullopt;
    }
    size_t counter = 1;
    for (size_t pos = first_open + 1; pos != text.length(); ++pos) {
        if (text[pos] == closing) {
            counter--;
        }
        else if (text[pos] == opening) {
            counter++;
        }
        if (counter == 0) {
            return std::make_pair(first_open, pos);
        }
    }
    return std::nullopt;
}

std::vector<std::string> split_into_words(const std::string& text)
{
    std::istringstream       ss{text};
    std::string              word;
    std::vector<std::string> res;
    while (ss >> word) {
        res.push_back(word);
    }
    return res;
}

} // namespace Cool::String