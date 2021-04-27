#include "../OpenGLWindowingSystem.h"

#include "GLDebugCallback.h"
#include <imgui/backends/imgui_impl_glfw.h>
#ifdef __COOL_APP_VULKAN
#include <imgui/backends/imgui_impl_vulkan.h>
#endif
#ifdef __COOL_APP_OPENGL
#include <imgui/backends/imgui_impl_opengl3.h>
#endif

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
#ifdef __COOL_APP_VULKAN
	ImGui_ImplVulkan_Shutdown();
#endif
#ifdef __COOL_APP_OPENGL
	ImGui_ImplOpenGL3_Shutdown();
#endif
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
	Log::Error("[Glfw] Error {} :\n{}", error, description);
}

OpenGLWindow OpenGLWindowingSystem::createWindow(const char* name, int width, int height, GLFWwindow* windowToShareContextWith) {
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
	OpenGLWindow openGlWindow(
		glfwCreateWindow(width, height, name, NULL, windowToShareContextWith)
	);
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
#ifdef __COOL_APP_VULKAN
	ImGui_ImplGlfw_InitForVulkan(openGLWindow.get(), true);
	// TODO
	// ImGui_ImplVulkan_InitInfo init_info = {};
	// init_info.Instance = g_Instance;
	// init_info.PhysicalDevice = g_PhysicalDevice;
	// init_info.Device = g_Device;
	// init_info.QueueFamily = g_QueueFamily;
	// init_info.Queue = g_Queue;
	// init_info.PipelineCache = g_PipelineCache;
	// init_info.DescriptorPool = g_DescriptorPool;
	// init_info.Allocator = g_Allocator;
	// init_info.MinImageCount = g_MinImageCount;
	// init_info.ImageCount = wd->ImageCount;
	// init_info.CheckVkResultFn = check_vk_result;
	// ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
#endif
#ifdef __COOL_APP_OPENGL
	ImGui_ImplGlfw_InitForOpenGL(openGLWindow.get(), true);
	std::string glslVersion = "#version " + std::to_string(m_openGLVersion);
	ImGui_ImplOpenGL3_Init(glslVersion.c_str());
#endif

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