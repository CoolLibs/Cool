#pragma once

#include <filesystem>
#include <tl/expected.hpp>

namespace Cool {

class File {
public:
    /**
     * @brief
     *
     * @return true iff file_path corresponds to an existing file or folder
     */
    static bool exists(std::string_view file_path);

    /**
     * @brief Converts a relative or absolute path to an absolute path
     */
    static std::string absolute_path(std::string_view file_path);

    /**
     * @brief
     *
     * @return The name of the file + its extension (removes parent folders)
     */
    static std::string file_name(std::string_view file_path);

    /**
     * @brief
     *
     * @return The name of the file (removes parent folders and extension)
     */
    static std::string file_name_without_extension(std::string_view file_path);

    /**
     * @brief
     *
     * @return The extension of the file, preceded by a dot : something like ".png"
     */
    static std::string extension(std::string_view file_path);

    /**
     * @brief The file_path but whithout the extension
     *
     * @return std::string
     */
    static std::string whithout_extension(std::string_view file_path);

    /**
     * @brief
     *
     * @return The folders in the path, removes the file name if there is one at the end
     */
    static std::string whithout_file_name(std::string_view file_path);

    /// Returns either an expected string that contains the content of the file, or an error string containing an error message explaining why the file couldn't be read.
    static tl::expected<std::string, std::string> to_string(std::string_view file_path);

    /**
     * @brief Recursively creates all the folders so that at the end folder_path is a valid folder path. /!\ There shouldn't be any file name at the end of folder_path ; it there is, then use create_folders_for_file_if_they_dont_exist() instead
     *
     * @return true iff the folders now exist (be it because they already existed or because they have been successfully created)
     */
    [[nodiscard("You can only use the folders if this function returns true")]] static bool
        create_folders_if_they_dont_exist(std::string_view folder_path);

    /**
     * @brief Recursively creates all the folders so that at the end file_path is a valid file path
     *
     * @return true iff the folders now exist (be it because they already existed or because they have been successfully created)
     */
    [[nodiscard("You can only use the file if this function returns true")]] static bool
        create_folders_for_file_if_they_dont_exist(std::string_view file_path);

    /**
     * @param folder_path The folder where you want the file to be created
     * @param file_name The name you would like to give to the file
     * @param extension The extension of the file, e.g. ".png"
     * @return std::string A file name that isn't in use yet ; it will be file_name, eventually postfixed with a number in parenthesis
     */
    static std::string find_available_name(std::string_view folder_path, std::string_view file_name, std::string_view extension);
};

} // namespace Cool