#ifdef __COOL_APP_OPENGL

#include "../../Renderer.h"
#include <Cool/App/Window.h>

namespace Cool {

void Renderer::clear_background(const glm::vec3& clear_color) {
	GLCall(glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::set_render_target(Window& window) {
	glfwMakeContextCurrent(window.get());
}

} // namespace Cool

#endif