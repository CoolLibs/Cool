#pragma once

#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Cool::Serialization {

class Exception : public std::exception {
public:
    explicit Exception(std::string_view message)
        : _message{message}
    {}

    auto what() const noexcept -> const char* override
    {
        return _message.c_str();
    }

private:
    std::string _message;
};

/**
 * @brief Loads data from a JSON file.
 *
 * @tparam T A type that implements the Cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html
 * @param data A reference to the data to initialize
 * @param file_path The path to the JSON file. If it doesn't exist this function will do nothing.
 */
template<typename T>
void from_json(T& data, std::filesystem::path file_path, bool is_allowed_to_throw = false)
{
    const auto on_error = [&](std::string_view message) {
        Log::ToUser::warning("Serialization::from_json", std::string{message});
        if (is_allowed_to_throw)
        {
            throw Exception{message};
        }
    };
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
            on_error(fmt::format("Invalid \"{}\" file. Starting with default values instead.\n{}", file_path.string(), e.what()));
        }
    }
    else
    {
        on_error(fmt::format("\"{}\" not found. Starting with default values instead.", file_path.string()));
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
