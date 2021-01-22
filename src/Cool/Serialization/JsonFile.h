#pragma once

#include <Cool/Log/Log.h>
#include <Cool/File/File.h>

namespace Cool::Serialization {

template <typename T>
void ToJSON(T& data, const char* filePath) {
	if (File::Exists(filePath)) {
		std::ifstream is(filePath);
		//{
			try {
				cereal::JSONInputArchive archive(is);
				archive(
					data
				);
			}
			catch (std::exception e) {
				Log::Release::Warn("Corrupted {} file. Starting with default values instead. \n{}", filePath, e.what());
			}
		//}
	}
	else {
		Log::Release::Warn("{} not found. Starting with default values instead.", filePath);
	}
}

template <typename T>
void FromJSON(T& data, const char* filePath) {
	File::CreateFoldersForFileIfDoesntExist(filePath);
	std::ofstream os(filePath);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			data
		);
	}
}

} // namespace Cool