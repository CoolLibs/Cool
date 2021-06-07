#include "Icons.h"

#include <Cool/LoadImage/LoadImage.h>
#include <Cool/OpenGL/Texture.h>
#include <filesystem>

namespace Cool {

std::unordered_map<std::string, GLuint> Icons::_map;

GLuint Icons::Get(std::string_view image_path) {
	const auto path = to_canonical_path(image_path);
	auto res = _map.find(path);
	if (res == _map.end()) {
		Log::Info("[Icons::Get] Generating texture for {}", path);
		GLuint tex_id = Texture::LoadTexture(path);
		_map[path] = tex_id;
		return tex_id;
	}
	else {
		return res->second;
	}
}

void Icons::CleanupTexture(std::string_view image_path) {
	const auto path = to_canonical_path(image_path);
	auto res = _map.find(path);
	if (res == _map.end()) {
		Log::Warn("[Icons::CleanupTexture] The texture you want to clean up doesn't exist ! {}", path);
	}
	else {
		Texture::DestroyTexture(res->second);
		_map.erase(path);
	}
}

std::string Icons::to_canonical_path(std::string_view path) {
	return std::filesystem::canonical(path).string();
}

} // namespace Cool