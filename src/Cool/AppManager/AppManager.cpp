#include "AppManager.h"
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Gpu/Vulkan/Context.h>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/imgui_internal.h>
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "Cool/UserSettings/UserSettings.h"
#include "GLFW/glfw3.h"
#include "should_we_use_a_separate_thread_for_update.h"

#if defined(COOL_VULKAN)
#include <imgui/backends/imgui_impl_vulkan.h>
#elif defined(COOL_OPENGL)
#include <imgui/backends/imgui_impl_opengl3.h>
#endif

namespace Cool {

static void prepare_windows(WindowManager& window_manager);
static void imgui_dockspace();
static void imgui_new_frame();
static void end_frame(WindowManager& window_manager);

AppManager::AppManager(WindowManager& window_manager, IApp& app, AppManagerConfig config)
    : _window_manager{window_manager}
    , _app{app}
    , _config{config}
{
    // Set callbacks
    for (auto& window : _window_manager.windows())
    {
        GLFWwindow* glfw_window = window.glfw();
        glfwSetWindowUserPointer(glfw_window, reinterpret_cast<void*>(this)); // NOLINT
        if (glfw_window != _window_manager.main_window().glfw())
        {
            glfwSetKeyCallback(glfw_window, AppManager::key_callback_for_secondary_windows);
            glfwSetWindowCloseCallback(glfw_window, AppManager::window_close_callback_for_secondary_windows);
        }
    }
    // clang-format off
    glfwSetKeyCallback        (_window_manager.main_window().glfw(), AppManager::key_callback);
    glfwSetMouseButtonCallback(_window_manager.main_window().glfw(), AppManager::mouse_button_callback);
    glfwSetScrollCallback     (_window_manager.main_window().glfw(), AppManager::scroll_callback);
    glfwSetCursorPosCallback  (_window_manager.main_window().glfw(), AppManager::cursor_position_callback);
    glfwSetCharCallback       (_window_manager.main_window().glfw(), ImGui_ImplGlfw_CharCallback);
    glfwSetWindowFocusCallback(_window_manager.main_window().glfw(), ImGui_ImplGlfw_WindowFocusCallback);
    glfwSetCursorEnterCallback(_window_manager.main_window().glfw(), ImGui_ImplGlfw_CursorEnterCallback);
    glfwSetMonitorCallback    (                                      ImGui_ImplGlfw_MonitorCallback);
    // clang-format on
}

void AppManager::run(std::function<void()> on_update)
{
#if defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD)
    auto should_stop   = false;
    auto update_thread = std::jthread{[&]() {
        NFD_Init();
        while (!glfwWindowShouldClose(_window_manager.main_window().glfw()))
        {
            update();
            on_update();
        }
        should_stop = true;
    }};
    while (!should_stop)
    {
        glfwWaitEvents();
    }
#else
    while (!glfwWindowShouldClose(_window_manager.main_window().glfw()))
    {
        glfwPollEvents();
        update();
        on_update();
    }
#endif
    // Restore any ImGui ini state that might have been stored
    _app._wants_to_restore_ini_state = true;
    restore_imgui_ini_state_ifn();
}

static void check_for_imgui_item_picker_request()
{
#if DEBUG
    if (DebugOptions::imgui_item_picker()
        || (ImGui::GetIO().KeyCtrl
            && ImGui::GetIO().KeyShift
            && ImGui::IsKeyPressed(ImGuiKey_I)
        ))
    {
        ImGui::DebugStartItemPicker();
    }
#endif
}

void AppManager::restore_imgui_ini_state_ifn()
{
    if (!_app._wants_to_restore_ini_state
        || !_app._imgui_ini_state_to_restore.has_value())
        return;

    ImGui::LoadIniSettingsFromMemory(_app._imgui_ini_state_to_restore->c_str());
    _app._wants_to_restore_ini_state = false;
    _app._imgui_ini_state_to_restore.reset();
}

void AppManager::update()
{
    ImGui::GetIO().ConfigDragClickToInputText = user_settings().single_click_to_input_in_drag_widgets;
    prepare_windows(_window_manager);
#if defined(COOL_VULKAN)
    vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
    TextureLibrary_FromWebcam::instance().on_frame_begin();
    if (TextureLibrary_FromWebcam::instance().has_active_webcam())
        _app.trigger_rerender();
    if (TextureLibrary_FromFile::instance().update())
        _app.trigger_rerender();
    _app.update();
    restore_imgui_ini_state_ifn(); // Must be done before imgui_new_frame() (this is a constraint from Dear ImGui (https://github.com/ocornut/imgui/issues/6263#issuecomment-1479727227))
    imgui_new_frame();
    check_for_imgui_item_picker_request();
    imgui_render(_app);
    TextureLibrary_FromWebcam::instance().on_frame_end();
    end_frame(_window_manager);
}

static void prepare_windows(WindowManager& window_manager)
{
#if defined(COOL_VULKAN)
    for (auto& window : window_manager.windows())
    {
        window.check_for_swapchain_rebuild();
    }
#elif defined(COOL_OPENGL)
    glfwMakeContextCurrent(window_manager.main_window().glfw());
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
#endif
}

static void imgui_new_frame()
{
#if defined(COOL_VULKAN)
    ImGui_ImplVulkan_NewFrame();
#elif defined(COOL_OPENGL)
    ImGui_ImplOpenGL3_NewFrame();
#endif
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    imgui_dockspace();
}

void AppManager::imgui_render(IApp& app)
{
    float window_title_height_bias = 0.f;

    // Apply normal font. The default font is the bold one because the window titles can only use the default font. We then have to push the regular() font back.
    window_title_height_bias += ImGui::GetFontSize();
    ImGui::PushFont(Font::regular());
    window_title_height_bias -= ImGui::GetFontSize();

    // Menu bar
    if (app.wants_to_show_menu_bar())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImGuiExtras::GetStyle().menu_bar_spacing);
        ImGui::BeginMainMenuBar();
        app.imgui_menus();
        ImGui::EndMainMenuBar();
        ImGui::PopStyleVar();
    }

    // Apply normal FramePadding. The one stored in ImGui::GetStyle() is used by the window titles only.
    window_title_height_bias += 2.f * ImGui::GetStyle().FramePadding.y;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImGuiExtras::GetStyle().frame_padding);
    window_title_height_bias -= 2.f * ImGui::GetStyle().FramePadding.y;

    ImGui::GetCurrentContext()->window_title_height_bias = window_title_height_bias;

    // Windows
    app.imgui_windows();
    imgui_windows();

    ImGui::PopStyleVar();
    ImGui::PopFont();
    ImGui::GetStyle().FramePadding = ImGuiExtras::GetStyle().tab_bar_padding; // We need to apply the tab_bar_padding here because simply changing it when the style editor UI changes it doesn't work because it is in the middle of the ImGui::PushStyleVar(ImGuiStyleVar_FramePadding) that we do above.
}

static void end_frame(WindowManager& window_manager)
{
    ImGui::Render();
#if defined(COOL_VULKAN)
    ImDrawData* main_draw_data    = ImGui::GetDrawData();
    const bool  main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
    if (!main_is_minimized)
    {
        window_manager.main_window().FrameRender(main_draw_data);
    }
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    if (!main_is_minimized)
    {
        window_manager.main_window().FramePresent();
    }
#elif defined(COOL_OPENGL)
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) // NOLINT
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(window_manager.main_window().glfw());
#endif
}

static void imgui_dockspace()
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) // NOLINT
    {
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

static WindowCoordinates mouse_position(GLFWwindow* window)
{
    double x, y; // NOLINT
    glfwGetCursorPos(window, &x, &y);
    return WindowCoordinates{x, y};
}

static AppManager& get_app_manager(GLFWwindow* window)
{
    return *reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window)); // NOLINT
}

void AppManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto& app_manager = get_app_manager(window);
    if (app_manager._config.dispatch_keyboard_events_to_imgui
        || ImGui::GetIO().WantTextInput) // We still want to allow shortcuts while in text input, like CTRL + SUPPR
    {
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    }
    app_manager._window_manager.main_window().check_for_fullscreen_toggles(key, scancode, action, mods);
    if (!ImGui::GetIO().WantTextInput && app_manager._app.inputs_are_allowed())
    {
        app_manager._app.on_keyboard_event({
            .key      = key,
            .scancode = scancode,
            .action   = action,
            .mods     = ModifierKeys{mods},
        });
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
    if (window.is_visible() && glfwWindowShouldClose(window.glfw()))
    {
        window.set_visibility(false);
        glfwSetWindowShouldClose(window.glfw(), GLFW_FALSE);
    }
}

void AppManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    auto& app_manager = get_app_manager(window);
    if (app_manager._app.inputs_are_allowed())
    {
        app_manager._app.on_mouse_button({
            .position = mouse_position(window),
            .button   = button,
            .action   = action,
            .mods     = ModifierKeys{mods},
        });
    }
}

void AppManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    auto& app_manager = get_app_manager(window);
    if (app_manager._app.inputs_are_allowed())
    {
        app_manager._app.on_mouse_scroll({
            .position = mouse_position(window),
            .dx       = static_cast<float>(xoffset),
            .dy       = static_cast<float>(yoffset),
        });
    }
}

void AppManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    auto& app_manager = get_app_manager(window);
    if (app_manager._app.inputs_are_allowed())
    {
        app_manager._app.on_mouse_move({.position = WindowCoordinates{xpos, ypos}});
    }
}

void AppManager::imgui_windows()
{
    Cool::DebugOptions::style_editor([&]() {
        _style_editor.imgui();
    });
}

} // namespace Cool