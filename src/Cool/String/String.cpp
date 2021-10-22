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

bool starts_with(std::string_view to_find, std::string_view str)
{
    return str.rfind(to_find, 0) == 0;
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
    const size_t next_pos = replace_next(in, 0);
    return "";
}

std::optional<std::string> find_replacement(std::string string_to_replace, const std::vector<std::pair<std::string, std::string>>& replacements)
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

size_t replace_next(const ReplacementInput& in, size_t start_pos)
{
    const auto begin = in.text.find("${", start_pos);
    if (begin == std::string::npos) {
        return std::string::npos;
    }
    else {
        const auto end = in.text.find("}", begin);
        if (end == std::string::npos) {
            throw std::invalid_argument{"No closing } found."};
        }
        else {
            return end;
        }
    }
}

std::string replace_at(size_t begin, size_t end, const ReplacementInput& in)
{
    const auto substr = in.text.substr(begin, end - begin);
    const auto res    = std::find_if(in.replacements.begin(), in.replacements.end(), [&](const std::pair<std::string, std::string>& pair) {
        return pair.first == substr;
    });
    return substr;
}

std::string replace_at(size_t begin, size_t end, const std::string& input_string, const std::string& new_substring)
{
    return input_string.substr(0, begin) + new_substring + input_string.substr(end, input_string.length() - end);
}

} // namespace Cool::String