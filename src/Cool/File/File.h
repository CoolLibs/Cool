#pragma once

#include <filesystem>

namespace Cool {

class File {
public:
      /**
       * @brief 
       * 
       * @param file_path 
       * @return true iff file_path corresponds to an existing file or folder
       */
      static bool exists(std::string_view file_path);

      /**
       * @brief Converts a relative or absolute path to an absolute path
       * 
       * @param file_path 
       * @return std::string 
       */
      static std::string absolute_path(std::string_view file_path);

      /**
       * @brief 
       * 
       * @param file_path 
       * @return The name of the file + its extension (removes parent folders)
       */
      static std::string file_name(const std::string& file_path);

      /**
       * @brief 
       * 
       * @param file_path 
       * @return The name of the file (removes parent folders and extension)
       */
      static std::string file_name_without_extension(const std::string& file_path);

      /**
       * @brief 
       * 
       * @param file_path 
       * @return The extension of the file, preceded by a dot : something like ".png"
       */
      static std::string extension(const std::string& file_path);

      /**
       * @brief The file_path but whithout the extension
       * 
       * @param file_path 
       * @return std::string 
       */
      static std::string whithout_extension(const std::string& file_path);

      /**
       * @brief 
       * 
       * @param file_path 
       * @return The folders in the path, removes the file name if there is one at the end
       */
      static std::string whithout_file_name(const std::string& file_path);
      
      /**
       * @brief Loads the content of a file into a std::string. (NB : We use this ugly out-param API instead of returning a string for performance reasons.)
       * 
       * @param file_path Path to the file to be loaded
       * @param dst Pointer to the string to be filled with the content of the file
       */
      static void to_string(std::string_view file_path, std::string* dst);

      /**
       * @brief Recursively creates all the folders so that at the end folder_path is a valid folder path. /!\ There shouldn't be any file name at the end of folder_path ; it there is, then use create_folders_for_file_if_they_dont_exist() instead
       * 
       * @param folder_path 
       * @return true iff the folders now exist (be it because they already existed or because they have been successfully created)
       */
      static bool create_folders_if_they_dont_exist(std::string_view folder_path);
      
      /**
       * @brief Recursively creates all the folders so that at the end file_path is a valid file path
       * 
       * @param file_path 
       * @return true iff the folders now exist (be it because they already existed or because they have been successfully created)
       */
      static bool create_folders_for_file_if_they_dont_exist(std::string_view file_path);

      /**
       * @brief Root directory where the program was started from. Use this to describe your ressources by an absolute path, especially if you play at runtime with the directories and your relative paths get messed up
       * 
       * @return 
       */
      static const std::string& root_dir();

      /**
       * @brief Initializes root_dir. This must be called once, at the start of your application
       * 
       * @param path 
       */
      static void initialize_root_dir(std::string_view path);

private:
      static std::string _root_dir;
#ifndef NDEBUG
      static bool _root_dir_is_initialized;
#endif
};

} // namespace Cool