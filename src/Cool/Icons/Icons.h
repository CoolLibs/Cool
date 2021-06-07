#pragma once

#include <Cool/File/File.h>

namespace Cool {

class Icons {
public:
	/// <summary></summary>
	/// <returns>The OpenGL texture ID of the "Play" icon</returns>
	static inline GLuint Play()    { return Get(File::root_dir() + "/Cool/Icons/icons/play.png"); }
	/// <summary></summary>
	/// <returns>The OpenGL texture ID of the "Pause" icon</returns>
	static inline GLuint Pause()   { return Get(File::root_dir() + "/Cool/Icons/icons/pause.png"); }
	/// <summary></summary>
	/// <returns>The OpenGL texture ID of the "Folder" icon</returns>
	static inline GLuint Folder() { return Get(File::root_dir() + "/Cool/Icons/icons/folder.png"); }

	/// <summary>
	/// Returns the OpenGL texture ID of the image. The first time Get is called for a given image path, the file is read and a texture is generated. Subsequent calls to Get for this same file will serve the texture ID from a cache (i.e. really fast).
	/// If you want to force textures to be preloaded, you can call Get("myImage.png") at the beginning of your app.
	/// /!\ Once Get is called and a texture is generated, it remains in GPU memory until the end of the application. If you don't need a texture anymore, you can use CleanupTexture().
	/// </summary>
	/// <param name="image_path">The path to the image file. Make sure you use an absolute path (e.g. File::RootDir + "/myPath/relativeTo/theExecutablesDirectory.png") to prevent problems that would arise when changing the working directory during the app's lifetime.</param>
	/// <returns></returns>
	static GLuint Get(std::string_view image_path);

	/// <summary>
	/// Destroys the OpenGL texture. It is recommended to only use this if you know you wont use that texture for a while and it is taking too much place in memory.
	/// </summary>
	/// <param name="image_path">The path to the image file. Make sure you use an absolute path (e.g. File::RootDir + "/myPath/relativeTo/theExecutablesDirectory.png") to prevent problems that would arise when changing the working directory during the app's lifetime.</param>
	static void CleanupTexture(std::string_view image_path);

private:
	static std::string to_canonical_path(std::string_view path);

private:
	static std::unordered_map<std::string, GLuint> _map;
};

} // namespace Cool