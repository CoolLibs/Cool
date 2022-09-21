#pragma once

#include <filesystem>
#include <tl/expected.hpp>

namespace Cool {

class File {
public:
    /// Returns true iff file_path corresponds to an existing file or folder
    static auto exists(std::filesystem::path file_path) -> bool;

    /// Returns the name of the file + its extension (removes parent folders)
    static auto file_name(std::filesystem::path file_path) -> std::filesystem::path;

    /// Returns the name of the file (removes parent folders and extension)
    static auto file_name_without_extension(std::filesystem::path file_path) -> std::filesystem::path;

    /// Returns the extension of the file, preceded by a dot: something like ".png"
    static auto extension(std::filesystem::path file_path) -> std::filesystem::path;

    /// Returns the file_path without its extension
    static auto without_extension(std::filesystem::path file_path) -> std::filesystem::path;

    /// Returns the folders in the path, removes the file name if there is one at the end
    static auto without_file_name(std::filesystem::path file_path) -> std::filesystem::path;

    /// Returns either an expected string that contains the content of the file, or an error string containing an error message explaining why the file couldn't be read.
    static auto to_string(std::filesystem::path file_path) -> tl::expected<std::string, std::string>;

    /// Recursively creates all the folders so that at the end `folder_path` is a valid folder path.
    /// /!\ There shouldn't be any file name at the end of folder_path; it there is, then use `create_folders_for_file_if_they_dont_exist()` instead.
    /// Returns true iff the folders now exist (be it because they already existed or because they have been successfully created).
    static auto create_folders_if_they_dont_exist(std::filesystem::path folder_path) -> bool [[nodiscard("You can only use the folders if this function returns true")]];

    /// Recursively creates all the folders so that at the end `file_path` is a valid file path.
    /// Returns true iff the folders now exist (be it because they already existed or because they have been successfully created).
    static auto create_folders_for_file_if_they_dont_exist(std::filesystem::path file_path) -> bool [[nodiscard("You can only use the file if this function returns true")]];

    /**
     * @param folder_path The folder where you want the file to be created
     * @param file_name The name you would like to give to the file
     * @param extension The extension of the file, e.g. ".png"
     * @return A file name that isn't in use yet; it will be file_name, eventually postfixed with a number in parenthesis
     */
    static auto find_available_name(std::filesystem::path folder_path, std::filesystem::path file_name, std::filesystem::path extension) -> std::filesystem::path;
};

} // namespace Cool