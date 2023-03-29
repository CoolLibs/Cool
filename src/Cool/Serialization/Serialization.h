#pragma once

#include <Cool/File/File.h>
#include <Cool/Log/OptionalErrorMessage.h>
#include <fstream>

namespace Cool::Serialization {

/**
 * @brief Loads data from a JSON file.
 *
 * @tparam T A type that implements the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html
 * @param data A reference to the data to initialize
 * @param file_path The path to the JSON file. If it doesn't exist this function will do nothing.
 */
template<typename T, typename InputArchive>
auto load(T& data, std::filesystem::path const& file_path) -> OptionalErrorMessage
{
    if (File::exists(file_path))
    {
        std::ifstream is(file_path);
        try
        {
            auto archive = InputArchive{is};
            archive(
                data
            );
        }
        catch (const std::exception& e)
        {
            return std::string_view{fmt::format("Invalid {} file.\nStarting with default values instead.\n{}", file_path, e.what())};
        }
    }
    else
    {
        return std::string_view{fmt::format("{} not found.\nStarting with default values instead.", file_path)};
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
template<typename T, typename OutputArchive>
void save(const T& data, std::filesystem::path const& file_path, std::string_view field_name = "value0")
{
    if (File::create_folders_for_file_if_they_dont_exist(file_path))
    {
        std::ofstream os(file_path);
        {
            auto archive = OutputArchive{os};
            archive(
                cereal::make_nvp(field_name.data(), data)
            );
        }
    }
}

} // namespace Cool::Serialization
