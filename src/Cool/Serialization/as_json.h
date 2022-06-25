#pragma once

#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Cool::Serialization {

/**
 * @brief Loads data from a JSON file.
 *
 * @tparam T A type that implements the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html
 * @param data A reference to the data to initialize
 * @param file_path The path to the JSON file. If it doesn't exist this function will do nothing.
 */
template<typename T>
void from_json(T& data, std::string_view file_path)
{
    if (File::exists(file_path))
    {
        std::ifstream is(file_path.data());
        try
        {
            cereal::JSONInputArchive archive(is);
            archive(
                data
            );
        }
        catch (const std::exception& e)
        {
            Log::ToUser::warn("Serialization::from_json", "Invalid \"{}\" file. Starting with default values instead.\n{}", file_path, e.what());
        }
    }
    else
    {
        Log::ToUser::warn("Serialization::from_json", "\"{}\" not found. Starting with default values instead.", file_path);
    }
}

/**
 * @brief Saves data to a JSON file.
 *
 * @tparam T A type that implements the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html
 * @param data The data to save
 * @param file_path The path to the JSON file. It will be created if it doesn't exist already.
 * @param field_name An optional name that will be given to data inside the JSON file (for readability purposes).
 */
template<typename T>
void to_json(const T& data, std::string_view file_path, std::string_view field_name = "value0")
{
    if (File::create_folders_for_file_if_they_dont_exist(file_path))
    {
        std::ofstream os(file_path.data());
        {
            cereal::JSONOutputArchive archive(os);
            archive(
                cereal::make_nvp(field_name.data(), data)
            );
        }
    }
}

} // namespace Cool::Serialization
