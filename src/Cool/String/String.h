#pragma once

namespace Cool::String {
	/**
	 * @brief 
	 * 
	 * @param str 
	 * @return A new string with all characters converted to lower case
	 */
	std::string to_lower(const std::string& str);

	/**
	 * @brief 
	 * 
	 * @param to_find 
	 * @param str 
	 * @return true iff *str* starts with *to_find*
	 */
	bool starts_with(const char* to_find, const std::string& str);

	/**
	 * @brief Modifies *str* by replacing all occurences of *from* with *to*
	 * 
	 * @param str 
	 * @param from 
	 * @param to 
	 */
	void replace_all(std::string& str, const std::string& from, const std::string& to);

	/**
	 * @brief Converts a number to a string. Adds 0s to the left until the size of the string is greater or equal to min_nb_of_characters.
	 * 
	 * @param val The number to convert to a string.
	 * @param min_nb_of_characters The minimum number of characters that will be present in the output string.
	 */
	template<typename T>
	std::string to_string(T val, int min_nb_of_characters = 0) {
		std::string str = std::to_string(val);
		int dn = min_nb_of_characters - static_cast<int>(str.size());
		if (dn > 0) {
			str = std::string(dn, '0') + str;
		}
		return str;
	}
} // namespace Cool::String