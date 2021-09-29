#pragma once

#include <filesystem>

namespace Cool {

class File {
public:
    /**
     * @brief Root directory where the program was started from. Use this to describe your ressources by an absolute path, especially if you play at runtime with the directories and your relative paths get messed up
     * 
     * @return 
     */
    static const std::string& root_dir();

    /**
     * @brief Initializes root_dir. This must be called once, at the start of your application
     * 
     */
    static void initialize_root_dir(std::string_view path);

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

    /**
     * @brief Reads the content of a file into a std::string.
     * 
     */
    static std::string to_string(std::string_view file_path);

    /**
     * @brief Recursively creates all the folders so that at the end folder_path is a valid folder path. /!\ There shouldn't be any file name at the end of folder_path ; it there is, then use create_folders_for_file_if_they_dont_exist() instead
     * 
     * @return true iff the folders now exist (be it because they already existed or because they have been successfully created)
     */
    static bool [[nodiscard]] create_folders_if_they_dont_exist(std::string_view folder_path);

    /**
     * @brief Recursively creates all the folders so that at the end file_path is a valid file path
     * 
     * @return true iff the folders now exist (be it because they already existed or because they have been successfully created)
     */
    static bool [[nodiscard]] create_folders_for_file_if_they_dont_exist(std::string_view file_path);

    /**
     * @param file_path The base file name
     * @return std::string A file name that isn't in use yet ; it will be file_path, eventually postfixed with a number in parenthesis
     */
    static std::string find_available_name(std::string_view file_path);

private:
    static std::string _root_dir;
#ifdef DEBUG
    static bool _root_dir_is_initialized;
#endif
};

} // namespace Cool