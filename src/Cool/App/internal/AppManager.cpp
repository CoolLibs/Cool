#include "../AppManager.h"

#include "../IApp.h"
#include "../RenderState.h"
#include "../Input.h"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

// Hide console in release builds
// msvc version
#if defined(_MSC_VER) && defined(NDEBUG)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

namespace Cool {

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{

}

static void window_size_callback(GLFWwindow* window, int w, int h)
{
	AppManager* appManager = reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
	appManager->onWindowResize(w, h);
	appManager->update();
}

static void window_pos_callback(GLFWwindow* window, int x, int y)
{
	AppManager* appManager = reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
	appManager->onWindowMove(x, y);
	appManager->update();
}

AppManager::AppManager(OpenGLWindow& mainWindow, IApp& app)
	: m_mainWindow(mainWindow), m_app(app)
{
	Input::Initialize();
	glfwSetKeyCallback(m_mainWindow.get(), AppManager::key_callback);
	glfwSetWindowSizeCallback(m_mainWindow.get(), window_size_callback);
	glfwSetWindowPosCallback(m_mainWindow.get(), window_pos_callback);
	glfwSetWindowUserPointer(m_mainWindow.get(), reinterpret_cast<void*>(this));
	int x, y, w, h;
	glfwGetWindowPos(m_mainWindow.get(), &x, &y);
	glfwGetWindowSize(m_mainWindow.get(), &w, &h);
	onWindowMove(x, y);
	onWindowResize(w, h);
}

void AppManager::run() {
	while (!glfwWindowShouldClose(m_mainWindow.get())) {
		update();
	}
}

void AppManager::onWindowMove(int x, int y) {
	RenderState::setWindowTopLeft(x, y);
}

void AppManager::onWindowResize(int w, int h) {
	RenderState::setWindowSize(w, h);
}

void AppManager::updateAvailableRenderingSpaceSizeAndPos(ImGuiDockNode* node) {
	// Position
	RenderState::setAvailableSpaceTopLeft(
		static_cast<int>(node->Pos.x) - RenderState::getWindowTopLeft().x,
		static_cast<int>(node->Pos.y) - RenderState::getWindowTopLeft().y
	);
	// Size
	glm::ivec2 size = { static_cast<int>(node->Size.x), static_cast<int>(node->Size.y) };
	if (size.x != RenderState::getAvailableSpaceSize().x || size.y != RenderState::getAvailableSpaceSize().y) {
		RenderState::setAvailableSpaceSize(size.x, size.y, !m_bFirstFrame);
	}
}

void AppManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	AppManager* appManager = reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
	appManager->m_app.onKeyboardEvent(key, scancode, action, mods);
}

bool AppManager::onEvent(const SDL_Event& e) {
	//if (m_sdlglWindow.checkForFullscreenToggles(e)) {
	//	onWindowResize();
	//	return false;
	//}
	/*
	switch (e.type) {

	case SDL_WINDOWEVENT:
		switch (e.window.event) {

		case SDL_WINDOWEVENT_RESIZED:
			onWindowResize();
			return false;

		case SDL_WINDOWEVENT_CLOSE:
			//if (e.window.windowID == SDL_GetWindowID(m_sdlglWindow.window)) {
			//	closeApp();
			//	return false;
			//}
			return false;

		case SDL_WINDOWEVENT_MOVED:
			onWindowMove();
			return false;

		default:
			return false;
		}

	case SDL_KEYDOWN:
		if (e.key.keysym.sym == 'h' && Input::KeyIsDown(SDL_SCANCODE_LCTRL)) {
			m_bShowUI = !m_bShowUI;
			return true;
		}
		return false;

	case SDL_QUIT:
		closeApp();
		return false;

	default:
		return false;
	}*/
	return false;
}

void AppManager::update() {
	// if (!onEvent(e))
	// 	app.onEvent(e);
	// Clear screen
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiDockspace();
	// Actual application code
	if (!m_bFirstFrame) // Don't update on first frame because RenderState::Size hasn't been initialized yet (we do this trickery to prevent the resizing event to be called twice at the start of the app)
		m_app.update();
	if (m_bShowUI) {
		// Menu bar
		if (!RenderState::IsExporting()) {
			ImGui::BeginMainMenuBar();
			if (ImGui::BeginMenu("Preview")) {
				RenderState::ImGuiPreviewControls();
				ImGui::EndMenu();
			}
			m_app.ImGuiMenus();
			ImGui::EndMainMenuBar();
		}
		// Windows
		m_app.ImGuiWindows();
	}
	// Render ImGui
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
	
	//// End frame
	m_bFirstFrame = false;
	glfwSwapBuffers(m_mainWindow.get());
	// Events
	glfwPollEvents();
}

void AppManager::ImGuiDockspace() {
	const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground
								  | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
								  | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
								  | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	bool bopen = true;
	ImGui::Begin("MyMainDockSpace", &bopen, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		updateAvailableRenderingSpaceSizeAndPos(ImGui::DockBuilderGetCentralNode(dockspace_id));
	}
	else
	{
		Log::Warn("Docking not enabled !");
	}
	ImGui::End();
}

} // namespace Cool