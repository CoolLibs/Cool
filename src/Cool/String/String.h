#pragma once

namespace Cool::String {
	/// <summary>
	/// Returns a new string with all characters converted to lower case
	/// </summary>
	std::string ToLower(const std::string& str);
	/// <summary>
	/// Returns true iff str starts with the string toFind
	/// </summary>
	bool StartsWith(const char* toFind, const std::string& str);
	/// <summary>
	/// Returns a new string with all occurences of "from" in "str" replaced with "to"
	/// </summary>
	void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
	/// <summary>
	/// Converts a number to a string.
	/// Adds 0s to the left until we reach minNbChars (if the latter isn't -1)
	/// </summary>
	template<typename T>
	std::string ToString(T val, int minNbChars = -1) {
		std::string str = std::to_string(val);
		if (minNbChars != -1) {
			int dn = minNbChars - static_cast<int>(str.size());
			if (dn > 0) {
				str = std::string(dn, '0') + str;
			}
		}
		return str;
	}
} // namespace Cool::String