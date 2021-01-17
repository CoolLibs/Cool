#include "SDLOpenGLWrapper.h"

#include "GLDebugCallback.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace Cool {

#ifndef NDEBUG
	bool SDLOpenGLWrapper::s_bInitialized = false;
#endif

SDLOpenGLWrapper::SDLOpenGLWrapper() {
	initializeSDLandOpenGL();
	initializeImGui();
#ifndef NDEBUG
	if (s_bInitialized)
		Log::Error("You are creating an SDLOpenGLWrapper twice !");
	s_bInitialized = true;
#endif
}

SDLOpenGLWrapper::~SDLOpenGLWrapper() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_Quit();
}

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

void SDLOpenGLWrapper::initializeImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
}

SDLGLWindow SDLOpenGLWrapper::createWindow(const char* name, int defaultWidth, int defaultHeight) {
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
	SDLGLWindow sdlglWindow(sdlWindow);
	sdlglWindow.makeCurrent();
#ifndef NDEBUG
	SDL_GL_SetSwapInterval(0);
#else
	SDL_GL_SetSwapInterval(1);
#endif
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		Log::Error("Failed to initialize Glad");
	setupGLDebugging();
	setupImGui(sdlglWindow);
	return sdlglWindow;
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

void SDLOpenGLWrapper::setupImGui(SDLGLWindow& sdlglWindow) {
	ImGui_ImplSDL2_InitForOpenGL(sdlglWindow.window, sdlglWindow.glContext);
	ImGui_ImplOpenGL3_Init("#version 430");
}

} // namespace Cool