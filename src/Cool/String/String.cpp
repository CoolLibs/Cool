#include "Cool/String/String.h"

std::string MyString::ToLower(const std::string& str) {
	std::string res = "";
	std::for_each(str.begin(), str.end(), [&res](char c) {
		res += std::tolower(c);
		});
	return res;
}

bool MyString::StartsWith(const char* toFind, const std::string& str) {
	return str.rfind(toFind, 0) == 0;
}

void MyString::ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}