#pragma once

#include "SDLGLWindow.h"

namespace Cool {

class IApp;

/// <summary>
/// Handles the initialization of SDL and OpenGL and the creation of windows.
/// </summary>
class SDLOpenGLWrapper {
public:
	SDLOpenGLWrapper();
	~SDLOpenGLWrapper();

	SDLGLWindow createWindow(const char* name, int defaultWidth, int defaultHeight);
	void setupImGui(SDLGLWindow& sdlglWindow);

private:
	void initializeSDLandOpenGL();
	void setupGLDebugging();
};

} // namespace Cool