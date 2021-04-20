#include "Icons.h"

#include <Cool/LoadImage/LoadImage.h>
#include <Cool/OpenGL/Texture.h>

namespace Cool {

std::unordered_map<std::string_view, GLuint> Icons::_map;

GLuint Icons::Get(std::string_view image_path) {
	auto res = _map.find(image_path);
	if (res == _map.end()) {
		Log::Info("[Icons::Get] Generating texture for {}", image_path);
		GLuint tex_id = Texture::LoadTexture(image_path);
		_map[image_path] = tex_id;
		return tex_id;
	}
	else {
		return res->second;
	}
}

void Icons::CleanupTexture(std::string_view image_path) {
	auto res = _map.find(image_path);
	if (res == _map.end()) {
		Log::Warn("[Icons::CleanupTexture] The texture you want to clean up doesn't exist ! {}", image_path);
	}
	else {
		Texture::DestroyTexture(res->second);
		_map.erase(image_path);
	}
}

} // namespace Cool