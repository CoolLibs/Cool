#include "../Input.h"

#include "../RenderState.h"

namespace Cool {

GLFWwindow* Input::s_mainWindow;
#ifndef NDEBUG
bool Input::s_initialized = false;
#endif

void Input::Initialize(GLFWwindow* mainWindow) {
	s_mainWindow = mainWindow;
#ifndef NDEBUG
	if (s_initialized)
		Log::Warn("[Input::Initialize] You are calling Initialize() twice.");
	s_initialized = true;
#endif
}

bool Input::MatchesChar(const char* character, int key) {
	const char* name = glfwGetKeyName(key, 0);
	return name && strcmp(name, character) == 0;
}

bool Input::KeyIsDown(int glfwKeyCode) {
	return glfwGetKey(s_mainWindow, glfwKeyCode) == GLFW_PRESS;
}

glm::vec2 Input::MouseInScreenCoordinates() {
	double x, y;
	glfwGetCursorPos(s_mainWindow, &x, &y);
	return glm::vec2(x, y) - glm::vec2(RenderState::InAppRenderArea().topLeft());
}

glm::vec2 Input::MouseInNormalizedRatioSpace() {
	glm::vec2 pos = MouseInScreenCoordinates();
	pos /= RenderState::InAppRenderArea().height();
	pos.y = 1.0f - pos.y;
	pos *= 2.0f;
	pos -= glm::vec2(RenderState::InAppRenderArea().aspectRatio(), 1.0f);
	return pos;
}

} // namespace Cool