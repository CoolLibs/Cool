#pragma once

#include <filesystem>

namespace Cool::File {

/// <summary>
/// Returns true iff filepath corresponds to an existing file or folder
/// </summary>
bool Exists(const char* filepath);
/// <summary>
/// Converts a relative or absolute path to an absolute path
/// </summary>
std::string AbsolutePath(const char* filepath);
/// <summary>
/// Returns the name of the file + its extension (removes parent folders)
/// </summary>
std::string FileName(const std::string& filepath);
/// <summary>
/// Returns the name of the file (removes parent folders and extension)
/// </summary>
std::string FileNameWithoutExtension(const std::string& filepath);
/// <summary>
/// Returns the extension of the file, preceded by a dot : something like ".png"
/// </summary>
std::string FileExtension(const std::string& filepath);
/// <summary>
/// Returns the filepath but whithout the extension.
/// </summary>
std::string WhithoutFileExtension(const std::string& filepath);
/// <summary>
/// Returns the folders in the path, removes the file name if there is one at the end
/// </summary>
std::string WhithoutFileName(const std::string& filepath);
/// <summary>
/// Loads the content of a file into a std::string.
/// (NB : We use this ugly API instead of returning a string for performance reasons.)
/// </summary>
/// <param name="filepath">Path to the file to be loaded</param>
/// <param name="dst">Pointer to the string to be filled with the content of the file</param>
void ToString(const char* filepath, std::string* dst);
/// <summary>
/// Recursively creates all the folders so that at the end folderPath is a valid folder path. /!\ There shouldn't be any file name at the end of folderPath ; it there is, then use CreateFoldersForFileIfDoesntExist() instead.
/// Returns true iff the folders now exist (be it because they already existed or because they have been successfully created)
/// </summary>
bool CreateFoldersIfDoesntExist(const char* folderPath);
/// <summary>
/// Recursively creates all the folders so that at the end folderAndFilePath is a valid file path.
/// Returns true iff the folders now exist (be it because they already existed or because they have been successfully created)
/// </summary>
bool CreateFoldersForFileIfDoesntExist(const char* folderAndFilePath);
/// <summary>
/// Root directory where the program was started from.
/// Use this to describe your ressources by an absolute path, especially if you play at runtime with the directories and your relative paths get messed up.
/// </summary>
const std::string RootDir = std::filesystem::current_path().string();

} // namespace Cool::File