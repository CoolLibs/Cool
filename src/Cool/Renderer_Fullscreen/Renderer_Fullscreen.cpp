#ifdef __COOL_APP_OPENGL

#include "Renderer_Fullscreen.h"
#include <Cool/App/RenderState.h>

namespace Cool {

Renderer_Fullscreen::Renderer_Fullscreen() {
	RenderState::SubscribeToSizeChanges([this]() { on_render_area_resized(); });
	GLDebug(glGenVertexArrays(1, &_dummy_vao_id));
}

Renderer_Fullscreen::~Renderer_Fullscreen() {
	GLDebug(glDeleteVertexArrays(1, &_dummy_vao_id));
}

void Renderer_Fullscreen::begin() {
	_render_buffer.bind();
}

void Renderer_Fullscreen::end() {
	_render_buffer.blitTo(
		RenderState::SwapYConvention(RenderState::InAppRenderArea().botLeft()),
		RenderState::SwapYConvention(RenderState::InAppRenderArea().topRight()),
		SCREEN_FRAMEBUFFER_ID,
		RenderState::preview_interpolation_mode()
	);
	_render_buffer.unbind();
}

void Renderer_Fullscreen::render() {
	// We use a smart trick to render fullscreen, as explained here : https://stackoverflow.com/a/59739538
	GLDebug(glBindVertexArray(_dummy_vao_id));
	GLDebug(glDrawArrays(GL_TRIANGLES, 0, 3));
}

void Renderer_Fullscreen::on_render_area_resized() {
	const glm::ivec2& size = RenderState::Size().size();
	if (size.x != 0 && size.y != 0)
		_render_buffer.setSize(size);
}

} // namespace Cool

#endif