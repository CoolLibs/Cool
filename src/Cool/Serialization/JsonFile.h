#pragma once

#include <Cool/Log/Log.h>
#include <Cool/File/File.h>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Cool::Serialization {

/// <summary>
/// Loads data from a JSON file. The type T needs to implement the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html
/// </summary>
/// <typeparam name="T">Any type that implements the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html </typeparam>
/// <param name="data">A reference to the data to initialize</param>
/// <param name="file_path">The path to the JSON file. If it doesn't exist this function will do nothing.</param>
template <typename T>
void from_json(T& data, std::string_view file_path) {
	if (File::Exists(file_path)) {
		std::ifstream is(file_path);
		try {
			cereal::JSONInputArchive archive(is);
			archive(
				data
			);
		}
		catch (std::exception e) {
			Log::ToUser::Warn("Invalid {} file. Starting with default values instead.\n{}", file_path, e.what());
		}
	}
	else {
		Log::ToUser::Warn("{} not found. Starting with default values instead.", file_path);
	}
}


/// <summary>
/// Saves data to a JSON file. The type T needs to implement the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html
/// </summary>
/// <typeparam name="T">Any type that implements the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html </typeparam>
/// <param name="data">The data to save</param>
/// <param name="file_path">The path to the JSON file. It will be created if it doesn't exist already.</param>
/// <param name="field_name">An optional name that will be given to data inside the JSON file (for readability purposes).</param>
template <typename T>
void to_json(const T& data, std::string_view file_path, std::string_view field_name = "value0") {
	File::CreateFoldersForFileIfDoesntExist(file_path);
	std::ofstream os(file_path);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			cereal::make_nvp(field_name.data(), data)
		);
	}
}

} // namespace Cool
