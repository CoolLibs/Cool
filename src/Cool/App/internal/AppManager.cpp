#include "../AppManager.h"

#include "../IApp.h"
#include "../RenderState.h"
#include "../Input.h"
#include <Cool/Gpu/Renderer.h>

#include <imgui/backends/imgui_impl_glfw.h>
#ifdef __COOL_APP_VULKAN
#include <imgui/backends/imgui_impl_vulkan.h>
#endif
#ifdef __COOL_APP_OPENGL
#include <imgui/backends/imgui_impl_opengl3.h>
#endif
#include <imgui/imgui_internal.h>

// Hide console in release builds
// msvc version
#if defined(_MSC_VER) && defined(NDEBUG)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

namespace Cool {

AppManager::AppManager(Window& mainWindow, IApp& app)
	: m_mainWindow(mainWindow), m_app(app)
{
	Input::Initialize(mainWindow.get());
	// Set callbacks
	glfwSetKeyCallback(m_mainWindow.get(), AppManager::key_callback);
	glfwSetMouseButtonCallback(m_mainWindow.get(), AppManager::mouse_button_callback);
	glfwSetScrollCallback(m_mainWindow.get(), AppManager::scroll_callback);
	glfwSetCursorPosCallback(m_mainWindow.get(), AppManager::cursor_position_callback);
	glfwSetWindowSizeCallback(m_mainWindow.get(), window_size_callback);
	glfwSetWindowPosCallback(m_mainWindow.get(), window_pos_callback);
	glfwSetWindowUserPointer(m_mainWindow.get(), reinterpret_cast<void*>(this));
	// Trigger window size / position event once
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

void AppManager::update() {
	// Events
	glfwPollEvents();
#ifdef __COOL_APP_VULKAN
	m_mainWindow.check_for_swapchain_rebuild();
#endif
	// Clear screen
	Renderer::set_render_target(m_mainWindow);
	Renderer::clear_background(RenderState::getEmptySpaceColor());
	// Start ImGui frame
#ifdef __COOL_APP_VULKAN
	ImGui_ImplVulkan_NewFrame();
#endif
#ifdef __COOL_APP_OPENGL
	ImGui_ImplOpenGL3_NewFrame();
#endif
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiDockspace();
	// Actual application code
	if (!m_bFirstFrame) // Don't update on first frame because RenderState::Size hasn't been initialized yet (we do this trickery to prevent the resizing event to be called twice at the start of the app)
		m_app.update();
	// UI
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
#ifdef __COOL_APP_VULKAN
	ImDrawData* main_draw_data = ImGui::GetDrawData();
	const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
	if (!main_is_minimized)
		m_mainWindow.FrameRender(main_draw_data);

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	// Present Main Platform Window
	if (!main_is_minimized)
		m_mainWindow.FramePresent();

#endif
#ifdef __COOL_APP_OPENGL
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
	glfwSwapBuffers(m_mainWindow.get());
#endif
	// End frame
	m_bFirstFrame = false;
}

void AppManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	AppManager* appManager = reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
	if (appManager->m_bDoForwardKeyEventsToImGui || ImGui::GetIO().WantTextInput)
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	// Fullscreen
	appManager->m_mainWindow.checkForFullscreenToggles(key, scancode, action, mods);
	// CTRL + H
	if (action == GLFW_RELEASE && Input::MatchesChar("h", key) && (mods & 2))
		appManager->m_bShowUI = !appManager->m_bShowUI;
	//
	appManager->m_app.onKeyboardEvent(key, scancode, action, mods);
}

void AppManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	AppManager* appManager = reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
	appManager->m_app.onMouseButtonEvent(button, action, mods);
}

void AppManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	AppManager* appManager = reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
	appManager->m_app.onScrollEvent(xoffset, yoffset);
}

void AppManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	AppManager* appManager = reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
	appManager->m_app.onMouseMoveEvent(xpos, ypos);
}

void AppManager::window_size_callback(GLFWwindow* window, int w, int h) {
	AppManager* appManager = reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
	appManager->onWindowResize(w, h);
	appManager->update();
}

void AppManager::window_pos_callback(GLFWwindow* window, int x, int y) {
	AppManager* appManager = reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
	appManager->onWindowMove(x, y);
	appManager->update();
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

void AppManager::ImGuiDockspace() {
	const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground
								  | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
								  | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
								  | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
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