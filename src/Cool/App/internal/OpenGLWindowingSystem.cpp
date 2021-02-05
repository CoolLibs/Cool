#include "../OpenGLWindowingSystem.h"

#include "GLDebugCallback.h"
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace Cool {

#ifndef NDEBUG
bool OpenGLWindowingSystem::s_bInitialized = false;
#endif

OpenGLWindowingSystem::OpenGLWindowingSystem(int openGLMajorVersion, int openGLMinorVersion)
	: m_openGLMajorVersion(openGLMajorVersion), m_openGLMinorVersion(openGLMinorVersion), m_openGLVersion(openGLMajorVersion * 100 + openGLMinorVersion * 10)
{
	// Debug checks
	assert(openGLMajorVersion >= 3);
	assert(openGLMinorVersion >= 3);
#ifndef NDEBUG
	if (s_bInitialized)
		Log::Error("You are creating an OpenGLWindowingSystem twice !");
	s_bInitialized = true;
#endif
	// Init
	initializeGLFW();
}

OpenGLWindowingSystem::~OpenGLWindowingSystem() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void OpenGLWindowingSystem::initializeGLFW() {
	glfwSetErrorCallback(OpenGLWindowingSystem::GlfwErrorCallback);
	if (!glfwInit()) {
		const char* errorDescription;
		glfwGetError(&errorDescription);
		Log::Error("Glfw initialization failed :\n{}", errorDescription);
	}
}

void OpenGLWindowingSystem::GlfwErrorCallback(int error, const char* description) {
	Log::Error("[Glfw] {}", description);
}

OpenGLWindow OpenGLWindowingSystem::createWindow(const char* name, int defaultWidth, int defaultHeight) {
	// Window flags
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_openGLMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_openGLMinorVersion);
#ifndef NDEBUG
	if (m_openGLVersion >= 430)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// Create window
	OpenGLWindow openGlWindow(glfwCreateWindow(defaultWidth, defaultHeight, name, NULL, NULL));
	if (!openGlWindow.get()) {
		const char* errorDescription;
		glfwGetError(&errorDescription);
		Log::Error("[Glfw] Window or OpenGL context creation failed :\n{}", errorDescription);
	}
	openGlWindow.makeCurrent();
	openGlWindow.enableVSync();
	// Load Glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		Log::Error("Failed to initialize Glad");
	//
	setupGLDebugging();
	setupImGui(openGlWindow);
	return openGlWindow;
}

void OpenGLWindowingSystem::setupGLDebugging() {
#ifndef NDEBUG
	if (m_openGLVersion >= 430) {
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
	}
#endif
}

void OpenGLWindowingSystem::setupImGui(OpenGLWindow& openGLWindow) {
	// Setup context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup style
	ImGui::StyleColorsClassic();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(openGLWindow.get(), true);
	std::string glslVersion = "#version " + std::to_string(m_openGLVersion);
	ImGui_ImplOpenGL3_Init(glslVersion.c_str());

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
}

} // namespace Cool