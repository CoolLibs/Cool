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
/// <param name="filePath">The path to the JSON file. If it doesn't exist this function will do nothing.</param>
template <typename T>
void FromJSON(T& data, std::string_view filePath) {
	if (File::Exists(filePath)) {
		std::ifstream is(filePath);
		try {
			cereal::JSONInputArchive archive(is);
			archive(
				data
			);
		}
		catch (std::exception e) {
			Log::Release::Warn("Invalid {} file. Starting with default values instead.\n{}", filePath, e.what());
		}
	}
	else {
		Log::Release::Warn("{} not found. Starting with default values instead.", filePath);
	}
}


/// <summary>
/// Saves data to a JSON file. The type T needs to implement the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html
/// </summary>
/// <typeparam name="T">Any type that implements the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html </typeparam>
/// <param name="data">The data to save</param>
/// <param name="filePath">The path to the JSON file. It will be created if it doesn't exist already.</param>
/// <param name="fieldName">An optional name that will be given to data inside the JSON file (for readability purposes).</param>
template <typename T>
void ToJSON(const T& data, std::string_view filePath, std::string_view fieldName = "value0") {
	File::CreateFoldersForFileIfDoesntExist(filePath);
	std::ofstream os(filePath);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			cereal::make_nvp(fieldName.data(), data)
		);
	}
}

} // namespace Cool
