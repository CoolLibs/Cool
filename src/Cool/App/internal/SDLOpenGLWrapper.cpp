#include "SDLOpenGLWrapper.h"

#include "Cool/Framework/GLDebugCallback.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace Cool {

void SDLOpenGLWrapper::initializeSDLandOpenGL() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		Log::Error("Failed to initialize SDL : {}", SDL_GetError());
	}
	SDL_GL_LoadLibrary(NULL);
#if __APPLE__
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
#ifndef NDEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
}

GLWindow SDLOpenGLWrapper::createGLWindow(const char* name, int defaultWidth, int defaultHeight) {
	SDL_Window* sdlWindow = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		defaultWidth, defaultHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
#ifdef NDEBUG
		| SDL_WINDOW_MAXIMIZED
#endif
	);
	if (sdlWindow == nullptr) {
		Log::Error("Failed to create window :\n{}", SDL_GetError());
	}
	GLWindow glWindow(sdlWindow);
	glWindow.makeCurrent();
	SDL_GL_SetSwapInterval(1);
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		Log::Error("Failed to initialize Glad");
	setupGLDebugging();
	setupImGui(glWindow);
	return glWindow;
}

void SDLOpenGLWrapper::setupGLDebugging() {
#ifndef NDEBUG
	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	else {
		Log::Warn("Couldn't setup OpenGL Debugging");
	}
#endif
}

SDLOpenGLWrapper::SDLOpenGLWrapper() {
	initializeSDLandOpenGL();

	// ------- Initialize ImGUI ------------

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

}

void SDLOpenGLWrapper::setupImGui(GLWindow& glWindow) {
	ImGui_ImplSDL2_InitForOpenGL(glWindow.window, glWindow.glContext);
	ImGui_ImplOpenGL3_Init("#version 430");
}


SDLOpenGLWrapper::~SDLOpenGLWrapper() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	//m_glWindows[0].destroy();
	SDL_Quit();
}

} // namespace Cool