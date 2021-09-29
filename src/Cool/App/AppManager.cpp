#include "AppManager.h"
#include <Cool/Input/Input.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include "IApp.h"
#ifdef __COOL_APP_VULKAN
#include <imgui/backends/imgui_impl_vulkan.h>
#endif
#ifdef __COOL_APP_OPENGL
#include <imgui/backends/imgui_impl_opengl3.h>
#endif
#include <imgui/imgui_internal.h>

#if defined(__COOL_APP_VULKAN)
#define COOL_UPDATE_ON_SEPARATE_THREAD
#endif

namespace Cool {

AppManager::AppManager(Window& mainWindow, WindowManager& window_manager, IApp& app)
    : _main_window(mainWindow)
    , _window_manager{window_manager}
    , m_app(app)
{
    // Set callbacks
    for (auto& window : _window_manager.windows()) {
        GLFWwindow* glfw_window = window.glfw();
        glfwSetWindowUserPointer(glfw_window, reinterpret_cast<void*>(this));
        if (glfw_window != _main_window.glfw()) {
            glfwSetKeyCallback(glfw_window, AppManager::key_callback_for_secondary_windows);
            glfwSetWindowCloseCallback(glfw_window, AppManager::window_close_callback_for_secondary_windows);
        }
    }
    // clang-format off
    glfwSetKeyCallback        (_main_window.glfw(), AppManager::key_callback);
    glfwSetMouseButtonCallback(_main_window.glfw(), AppManager::mouse_button_callback);
    glfwSetScrollCallback     (_main_window.glfw(), AppManager::scroll_callback);
    glfwSetCursorPosCallback  (_main_window.glfw(), AppManager::cursor_position_callback);
    glfwSetCharCallback       (_main_window.glfw(), ImGui_ImplGlfw_CharCallback);
    glfwSetWindowFocusCallback(_main_window.glfw(), ImGui_ImplGlfw_WindowFocusCallback);
    glfwSetCursorEnterCallback(_main_window.glfw(), ImGui_ImplGlfw_CursorEnterCallback);
    glfwSetMonitorCallback    (                     ImGui_ImplGlfw_MonitorCallback);
    // clang-format on
}

AppManager::~AppManager()
{
#if defined(COOL_UPDATE_ON_SEPARATE_THREAD)
    if (_update_thread.joinable()) {
        _update_thread.join();
    }
#endif
}

void AppManager::run()
{
#if defined(COOL_UPDATE_ON_SEPARATE_THREAD)
    _update_thread = std::thread{[this]() {
        NFD_Init();
        while (!glfwWindowShouldClose(_main_window.glfw())) {
            update();
        }
    }};
    while (!glfwWindowShouldClose(_main_window.glfw())) {
        glfwWaitEvents();
    }
#else
    while (!glfwWindowShouldClose(_main_window.glfw())) {
        glfwPollEvents();
        update();
    }
#endif
}

void AppManager::update()
{
#ifdef __COOL_APP_VULKAN
    for (auto& window : _window_manager.windows()) {
        window.check_for_swapchain_rebuild();
    }
#endif
#ifdef __COOL_APP_OPENGL
    _main_window.make_current();
#endif
    // Actual application code
    m_app.update();
    // Start ImGui frame
#ifdef __COOL_APP_VULKAN
    ImGui_ImplVulkan_NewFrame();
#endif
#ifdef __COOL_APP_OPENGL
    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
#endif
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiDockspace();
    // UI
    if (m_bShowUI) {
        // Menu bar
        if (m_app.wants_to_show_menu_bar()) {
            ImGui::BeginMainMenuBar();
            m_app.ImGuiMenus();
            ImGui::EndMainMenuBar();
        }
        // Windows
        m_app.ImGuiWindows();
    }
    // Render ImGui
    ImGui::Render();
#ifdef __COOL_APP_VULKAN
    ImDrawData* main_draw_data    = ImGui::GetDrawData();
    const bool  main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
    if (!main_is_minimized)
        _main_window.FrameRender(main_draw_data);

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    // Present Main Platform Window
    if (!main_is_minimized)
        _main_window.FramePresent();

#endif
#ifdef __COOL_APP_OPENGL
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(_main_window.glfw());
#endif
}

static WindowCoordinates mouse_position(GLFWwindow* window)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return WindowCoordinates{x, y};
}

static AppManager& get_app_manager(GLFWwindow* window)
{
    return *reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window));
}

void AppManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto& app_manager = get_app_manager(window);
    if (app_manager.m_bDoForwardKeyEventsToImGui || ImGui::GetIO().WantTextInput) {
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    }
    app_manager._main_window.check_for_fullscreen_toggles(key, scancode, action, mods);
    if (action == GLFW_RELEASE && Input::matches_char("h", key) && (mods & GLFW_MOD_CONTROL)) {
        app_manager.m_bShowUI = !app_manager.m_bShowUI;
    }
    if (!ImGui::GetIO().WantTextInput && app_manager.m_app.inputs_are_allowed()) {
        app_manager.m_app.on_keyboard_event({.key      = key,
                                             .scancode = scancode,
                                             .action   = action,
                                             .mods     = mods});
    }
}

void AppManager::key_callback_for_secondary_windows(GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
{
    get_app_manager(glfw_window)
        ._window_manager.find(glfw_window)
        .check_for_fullscreen_toggles(key, scancode, action, mods);
}

void AppManager::window_close_callback_for_secondary_windows(GLFWwindow* glfw_window)
{
    Window& window = get_app_manager(glfw_window)._window_manager.find(glfw_window);
    if (window.is_visible() && glfwWindowShouldClose(window.glfw())) {
        window.set_visibility(false);
        glfwSetWindowShouldClose(window.glfw(), GLFW_FALSE);
    }
}

void AppManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    auto& app_manager = get_app_manager(window);
    if (app_manager.m_app.inputs_are_allowed()) {
        app_manager.m_app.on_mouse_button({.position = mouse_position(window),
                                           .button   = button,
                                           .action   = action,
                                           .mods     = mods});
    }
}

void AppManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    auto& app_manager = get_app_manager(window);
    if (app_manager.m_app.inputs_are_allowed()) {
        app_manager.m_app.on_mouse_scroll({.position = mouse_position(window),
                                           .dx       = static_cast<float>(xoffset),
                                           .dy       = static_cast<float>(yoffset)});
    }
}

void AppManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    auto& app_manager = get_app_manager(window);
    if (app_manager.m_app.inputs_are_allowed()) {
        app_manager.m_app.on_mouse_move({.position = WindowCoordinates{xpos, ypos}});
    }
}

void AppManager::ImGuiDockspace()
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        constexpr ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        constexpr ImGuiWindowFlags   window_flags    = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("MyMainDockSpace", nullptr, window_flags);
        ImGui::PopStyleVar(3);
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        ImGui::End();
    }
}

} // namespace Cool