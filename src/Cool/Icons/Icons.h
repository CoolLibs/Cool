#pragma once

namespace Cool {

class Icons {
public:
	/// <summary></summary>
	/// <returns>The OpenGL texture ID of the "Play" icon</returns>
	static inline GLuint Play()  { return Get("Cool/Icons/icons/play.png"); }
	/// <summary></summary>
	/// <returns>The OpenGL texture ID of the "Pause" icon</returns>
	static inline GLuint Pause() { return Get("Cool/Icons/icons/pause.png"); }

	/// <summary>
	/// Returns the OpenGL texture ID of the image. The first time Get is called for a given image path, the file is read and a texture is generated. Subsequent calls to Get for this same file will serve the texture ID from a cache (i.e. really fast).
	/// If you want to force textures to be preloaded, you can call Get("myImage.png") at the beginning of your app.
	/// /!\ Once Get is called and a texture is generated, it remains in GPU memory until the end of the application. If you don't need a texture anymore, you can use CleanupTexture().
	/// </summary>
	/// <param name="image_path">The file path to the</param>
	/// <returns></returns>
	static GLuint Get(std::string_view image_path);

	/// <summary>
	/// Destroys the OpenGL texture. It is recommended to only use this if you know you wont use that texture for a while and it is taking too much place in memory.
	/// </summary>
	/// <param name="image_path"></param>
	static void CleanupTexture(std::string_view image_path);

private:
	static std::unordered_map<std::string_view, GLuint> _map;
};

} // namespace Cool