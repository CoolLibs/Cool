#pragma once

#include <filesystem>

namespace Cool {

class File {
public:
      /// <summary>
      /// Returns true iff filepath corresponds to an existing file or folder
      /// </summary>
      static bool Exists(std::string_view filepath);
      /// <summary>
      /// Converts a relative or absolute path to an absolute path
      /// </summary>
      static std::string AbsolutePath(std::string_view filepath);
      /// <summary>
      /// Returns the name of the file + its extension (removes parent folders)
      /// </summary>
      static std::string FileName(const std::string& filepath);
      /// <summary>
      /// Returns the name of the file (removes parent folders and extension)
      /// </summary>
      static std::string FileNameWithoutExtension(const std::string& filepath);
      /// <summary>
      /// Returns the extension of the file, preceded by a dot : something like ".png"
      /// </summary>
      static std::string FileExtension(const std::string& filepath);
      /// <summary>
      /// Returns the filepath but whithout the extension.
      /// </summary>
      static std::string WhithoutFileExtension(const std::string& filepath);
      /// <summary>
      /// Returns the folders in the path, removes the file name if there is one at the end
      /// </summary>
      static std::string WhithoutFileName(const std::string& filepath);
      /// <summary>
      /// Loads the content of a file into a std::string.
      /// (NB : We use this ugly API instead of returning a string for performance reasons.)
      /// </summary>
      /// <param name="filepath">Path to the file to be loaded</param>
      /// <param name="dst">Pointer to the string to be filled with the content of the file</param>
      static void ToString(std::string_view filepath, std::string* dst);
      /// <summary>
      /// Recursively creates all the folders so that at the end folderPath is a valid folder path. /!\ There shouldn't be any file name at the end of folderPath ; it there is, then use CreateFoldersForFileIfDoesntExist() instead.
      /// Returns true iff the folders now exist (be it because they already existed or because they have been successfully created)
      /// </summary>
      static bool CreateFoldersIfDoesntExist(std::string_view folderPath);
      /// <summary>
      /// Recursively creates all the folders so that at the end folderAndFilePath is a valid file path.
      /// Returns true iff the folders now exist (be it because they already existed or because they have been successfully created)
      /// </summary>
      static bool CreateFoldersForFileIfDoesntExist(std::string_view folderAndFilePath);
      /// <summary>
      /// Root directory where the program was started from.
      /// Use this to describe your ressources by an absolute path, especially if you play at runtime with the directories and your relative paths get messed up.
      /// </summary>
      static const std::string& root_dir();

      static void initialize_root_dir(std::string_view path);

private:
      static std::string _root_dir;
#ifndef NDEBUG
      static bool _root_dir_is_initialized;
#endif
};

} // namespace Cool