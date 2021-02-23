#pragma once

#include <Cool/Log/Log.h>
#include <Cool/File/File.h>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Cool::Serialization {

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

template <typename T>
void ToJSON(T& data, std::string_view filePath, std::string_view fieldName = "value0") {
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
