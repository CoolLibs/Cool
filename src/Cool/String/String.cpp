#include "String.h"

namespace Cool::String {

std::string to_lower(std::string_view str) {
	std::string res = "";
	std::for_each(str.begin(), str.end(), [&res](char c) {
		res += std::tolower(c);
	});
	return res;
}

bool starts_with(std::string_view toFind, std::string_view str) {
	return str.rfind(toFind, 0) == 0;
}

void replace_all(std::string& str, std::string_view from,std::string_view to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

} // namespace Cool::String