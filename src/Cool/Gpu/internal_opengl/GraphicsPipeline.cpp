#pragma once

namespace Cool {

class Window;

class Renderer {
public:
	static void clear_background(const glm::vec3& clear_color);
	static void set_render_target(Window& window);

private:
#ifdef __COOL_APP_VULKAN
	static Window* _window;
#endif
};

} // namespace Cool