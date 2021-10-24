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
        next = replace_next({next.first, in.replacements}, *next.second);
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
    const auto begin = in.text.find("${", start_pos);
    if (begin == std::string::npos) {
        return std::make_pair(in.text, std::nullopt);
    }
    else {
        const auto end = in.text.find("}", begin);
        if (end == std::string::npos) {
            throw std::invalid_argument{"No closing } found."};
        }
        else {
            const auto replacement_begin = begin + 2;
            const auto to_replace        = in.text.substr(replacement_begin, end - replacement_begin);
            const auto replacement       = find_replacement(to_replace, in.replacements);
            if (replacement.has_value()) {
                return std::make_pair(replace_at(begin, end + 1, in.text, *replacement),
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

} // namespace Cool::String