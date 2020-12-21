#pragma once

#include "GLWindow.h"

namespace Cool {

class IApp;

/// <summary>
/// Handles the initialization of SDL and OpenGL and the creation of windows.
/// </summary>
class SDLOpenGLWrapper {
public:
	SDLOpenGLWrapper();
	~SDLOpenGLWrapper();

	GLWindow createGLWindow(const char* name, int defaultWidth, int defaultHeight);
	void setupImGui(GLWindow& glWindow);

private:
	void initializeSDLandOpenGL();
	void setupGLDebugging();
};

} // namespace Cool