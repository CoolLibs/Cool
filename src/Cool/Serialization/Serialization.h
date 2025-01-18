#pragma once

#include <Cool/File/File.h>
#include <Cool/Log/OptionalErrorMessage.h>
#include <fstream>
#include <string>

namespace Cool::Serialization {

/**
 * @brief Loads data from a JSON file.
 *
 * @tparam T A type that implements the cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html
 * @param data A reference to the data to initialize
 * @param file_path The path to the JSON file. If it doesn't exist this function will do nothing.
 */
template<typename T, typename InputArchive>
auto load(T& data, std::filesystem::path const& file_path, std::string* extra_line_at_the_beginning_of_the_file = {}) -> OptionalErrorMessage
{
    if (!File::exists(file_path))
        return fmt::format("File not found \"{}\"", Cool::File::weakly_canonical(file_path));

    std::ifstream ifs{file_path};
    if (extra_line_at_the_beginning_of_the_file != nullptr)
        std::getline(ifs, *extra_line_at_the_beginning_of_the_file);
    try
    {
        auto archive = InputArchive{ifs};
        archive(data);
    }
    catch (std::exception const& e)
    {
        return fmt::format("Corrupted file \"{}\":\n{}", Cool::File::weakly_canonical(file_path), e.what());
    }
    return {};
}

/**
 * @brief Saves data to a JSON file.
 *
 * @tparam T A type that implements the cereal serialization functions described here : https://uscilab.github.io/cereal/serialization_functions.html
 * @param data The data to save
 * @param file_path The path to the JSON file. It will be created if it doesn't exist already.
 * @param field_name An optional name that will be given to data inside the JSON file (for readability purposes).
 * return true iff the save was successful.
 */
template<typename T, typename OutputArchive>
auto save(const T& data, std::filesystem::path const& file_path, std::string_view field_name = "value0", std::optional<std::string> const& extra_line_at_the_beginning_of_the_file = {}) -> bool
{
    if (!File::create_folders_for_file_if_they_dont_exist(file_path))
        return false;

    auto os = std::ofstream{file_path};
    if (!os.is_open())
        return false;

    if (extra_line_at_the_beginning_of_the_file.has_value())
    {
        assert(extra_line_at_the_beginning_of_the_file->find('\n') == std::string::npos); // extra_line_at_the_beginning_of_the_file must be a single line, with no \n at the end
        os << extra_line_at_the_beginning_of_the_file.value() << '\n';
    }
    {
        auto archive = OutputArchive{os};
        archive(
            ser20::make_nvp(field_name.data(), data)
        );
    }
    return true;
}

} // namespace Cool::Serialization
