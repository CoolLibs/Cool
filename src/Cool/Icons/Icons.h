#pragma once

#include <Cool/File/File.h>

namespace Cool {

class Icons {
public:
    /**
	 * @brief 
	 * 
	 * @return The OpenGL texture ID of the "play" icon 
	 */
    static inline GLuint play() { return get(File::root_dir() + "/Cool/res/icons/play.png"); }

    /**
	 * @brief 
	 * 
	 * @return The OpenGL texture ID of the "pause" icon 
	 */
    static inline GLuint pause() { return get(File::root_dir() + "/Cool/res/icons/pause.png"); }

    /**
	 * @brief 
	 * 
	 * @return The OpenGL texture ID of the "folder" icon 
	 */
    static inline GLuint folder() { return get(File::root_dir() + "/Cool/res/icons/folder.png"); }

    /**
	 * @brief Returns the OpenGL texture ID of the image. The first time get() is called for a given image path, the file is read and a texture is generated. Subsequent calls to get() for this same file will serve the texture ID from a cache (i.e. really fast). If you want to force textures to be preloaded, you can call get("myImage.png") at the beginning of your app. /!\ Once get() is called and a texture is generated, it remains in GPU memory until the end of the application. If you don't need a texture anymore, you can use cleanup_texture().
	 * 
	 * @param image_path The path to the image file. Make sure you use an absolute path (e.g. File::RootDir + "/myPath/relativeTo/theExecutablesDirectory.png") to prevent problems that would arise when changing the working directory during the app's lifetime
	 * @return The OpenGL texture ID of the image. The first time get is called for a given image path, the file is read and a texture is generated. Subsequent calls to get for this same file will serve the texture ID from a cache (i.e. really fast). If you want to force textures to be preloaded, you can call get("myImage.png") at the beginning of your app.
	 */
    static GLuint get(std::string_view image_path);

    /**
	 * @brief Destroys the OpenGL texture. It is recommended to only use this if you know you won't use that texture for a while and it is taking too much place in memory
	 * 
	 * @param image_path The path to the image file. Make sure you use an absolute path (e.g. File::RootDir + "/myPath/relativeTo/theExecutablesDirectory.png") to prevent problems that would arise when changing the working directory during the app's lifetime
	 */
    static void cleanup_texture(std::string_view image_path);

private:
    static std::unordered_map<std::string, GLuint> _map;
};

} // namespace Cool