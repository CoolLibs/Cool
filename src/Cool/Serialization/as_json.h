#pragma once

#include <Cool/File/File.h>
#include <Cool/Log/OptionalErrorMessage.h>
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
auto from_json(T& data, std::filesystem::path file_path) -> OptionalErrorMessage
{
    if (File::exists(file_path.string()))
    {
        std::ifstream is(file_path);
        try
        {
            cereal::JSONInputArchive archive(is);
            archive(
                data
            );
        }
        catch (const std::exception& e)
        {
            return std::string_view{fmt::format("Invalid \"{}\" file.\nStarting with default values instead.\n{}", file_path.string(), e.what())};
        }
    }
    else
    {
        return std::string_view{fmt::format("\"{}\" not found.\nStarting with default values instead.", file_path.string())};
    }
    return {};
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
void to_json(const T& data, std::filesystem::path file_path, std::string_view field_name = "value0")
{
    if (File::create_folders_for_file_if_they_dont_exist(file_path.string()))
    {
        std::ofstream os(file_path);
        {
            cereal::JSONOutputArchive archive(os);
            archive(
                cereal::make_nvp(field_name.data(), data)
            );
        }
    }
}

} // namespace Cool::Serialization
