#include "AppManager.h"
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Gpu/Vulkan/Context.h>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_wgpu.h>
#include <imgui/imgui_internal.h>
#include <fix_tdr_delay/fix_tdr_delay.hpp>
#include "Audio/Audio.hpp"
#include "Cool/Backend/Window.h"
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/Gpu/WebGPUContext.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "Cool/Input/MouseButtonEvent.h"
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Midi/MidiManager.h"
#include "Cool/UserSettings/UserSettings.h"
#include "Cool/Webcam/TextureLibrary_FromWebcam.h"
#include "GLFW/glfw3.h"
#include "nfd.hpp"
#include "should_we_use_a_separate_thread_for_update.h"

namespace Cool {

static void imgui_dockspace();
static void imgui_new_frame();

static AppManager& get_app_manager(GLFWwindow* window)
{
    return *reinterpret_cast<AppManager*>(glfwGetWindowUserPointer(window)); // NOLINT
}

void AppManager::key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
{
    auto& app_manager = get_app_manager(glfw_window);
    if (app_manager._config.dispatch_keyboard_events_to_imgui
        || ImGui::GetIO().WantTextInput) // We still want to allow shortcuts while in text input, like CTRL + SUPPR
    {
        ImGui_ImplGlfw_KeyCallback(glfw_window, key, scancode, action, mods);
    }
    window().check_for_fullscreen_toggles(key, scancode, action, mods);
}

static void resize_callback(GLFWwindow* /* glfw_window */, int width, int height)
{
    webgpu_context().on_window_size_changed(width, height);
}

AppManager::AppManager(ViewsManager& views, IApp& app, AppManagerConfig config)
    : _views{views}
    , _app{app}
    , _config{config}
{
    fix_tdr_delay::set_minimum_delay(60); // Fixes a GPU crash on Windows. See the documentation of the library: https://github.com/CoolLibs/fix-tdr-delay

    // Set callbacks
    // clang-format off
    glfwSetWindowUserPointer      (window().glfw(), reinterpret_cast<void*>(this));
    glfwSetKeyCallback            (window().glfw(), AppManager::key_callback);
    glfwSetMouseButtonCallback    (window().glfw(), ImGui_ImplGlfw_MouseButtonCallback);
    glfwSetScrollCallback         (window().glfw(), ImGui_ImplGlfw_ScrollCallback);
    glfwSetCursorPosCallback      (window().glfw(), ImGui_ImplGlfw_CursorPosCallback);
    glfwSetCharCallback           (window().glfw(), ImGui_ImplGlfw_CharCallback);
    glfwSetWindowFocusCallback    (window().glfw(), ImGui_ImplGlfw_WindowFocusCallback);
    glfwSetCursorEnterCallback    (window().glfw(), ImGui_ImplGlfw_CursorEnterCallback);
    glfwSetMonitorCallback        (                 ImGui_ImplGlfw_MonitorCallback);
    glfwSetFramebufferSizeCallback(window().glfw(), resize_callback);
    // clang-format on
}

void AppManager::run(std::function<void()> const& on_update)
{
    auto const do_update = [&]() {
#if !DEBUG
        try
#endif
        {
            update();
            on_update();
        }
#if !DEBUG
        catch (std::exception const& e)
        {
            Cool::Log::ToUser::error("UNKNOWN ERROR 1", e.what());
        }
#endif
    };
#if defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD)
    auto should_stop   = false;
    auto update_thread = std::jthread{[&]() {
        NFD::Guard nfd_guard{};
        while (!glfwWindowShouldClose(_window().glfw()))
        {
            do_update();
        }
        should_stop = true;
    }};
    while (!should_stop)
    {
        glfwWaitEvents();
    }
#else
    NFD::Guard nfd_guard{};
    while (!glfwWindowShouldClose(window().glfw()))
    {
        glfwPollEvents();
        do_update();
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
    wgpu::TextureView nextTexture             = webgpu_context().swapChain.getCurrentTextureView();
    if (!nextTexture)
    {
        std::cerr << "Cannot acquire next swap chain texture" << std::endl; // TODO(WebGPU) Can this legitimately happen, ot is this always an error we need to handle / report

        return; // TODO(WebGPU) We still want to update, but not render
    }
#if defined(COOL_VULKAN)
    vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif

    wgpu::CommandEncoderDescriptor commandEncoderDesc;
    commandEncoderDesc.label     = "Command Encoder";
    wgpu::CommandEncoder encoder = webgpu_context().device.createCommandEncoder(commandEncoderDesc);

    wgpu::RenderPassDescriptor renderPassDesc{};

    wgpu::RenderPassColorAttachment renderPassColorAttachment{};
    renderPassColorAttachment.view          = nextTexture;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp        = wgpu::LoadOp::Clear;
    renderPassColorAttachment.storeOp       = wgpu::StoreOp::Store;
    renderPassColorAttachment.clearValue    = wgpu::Color{1., 0.05, 0.05, 1.0};
    renderPassDesc.colorAttachmentCount     = 1;
    renderPassDesc.colorAttachments         = &renderPassColorAttachment;

    wgpu::RenderPassDepthStencilAttachment depthStencilAttachment;
    depthStencilAttachment.view              = webgpu_context().depthTextureView;
    depthStencilAttachment.depthClearValue   = 1.0f;
    depthStencilAttachment.depthLoadOp       = wgpu::LoadOp::Clear;
    depthStencilAttachment.depthStoreOp      = wgpu::StoreOp::Store;
    depthStencilAttachment.depthReadOnly     = false;
    depthStencilAttachment.stencilClearValue = 0;
#ifdef WEBGPU_BACKEND_WGPU
    depthStencilAttachment.stencilLoadOp  = wgpu::LoadOp::Clear;
    depthStencilAttachment.stencilStoreOp = wgpu::StoreOp::Store;
#else
    depthStencilAttachment.stencilLoadOp  = wgpu::LoadOp::Undefined;
    depthStencilAttachment.stencilStoreOp = wgpu::StoreOp::Undefined;
#endif
    depthStencilAttachment.stencilReadOnly = true;

    renderPassDesc.depthStencilAttachment = &depthStencilAttachment;

    renderPassDesc.timestampWriteCount = 0;
    renderPassDesc.timestampWrites     = nullptr;
    webgpu_context().mainRenderPass    = encoder.beginRenderPass(renderPassDesc);

    midi_manager().check_for_devices();
    Audio::player().update_device_if_necessary();
    TextureLibrary_FromWebcam::instance().on_frame_begin();
    if (TextureLibrary_FromWebcam::instance().has_active_webcams())
        _app.request_rerender();
    if (TextureLibrary_FromFile::instance().update()) // update() needs to be called because update has side effect
        _app.request_rerender();
#if !DEBUG
    try
#endif
    {
        _app.update();
    }
#if !DEBUG
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("UNKNOWN ERROR 2", e.what());
    }
#endif
    restore_imgui_ini_state_ifn(); // Must be before `imgui_new_frame()` (this is a constraint from Dear ImGui (https://github.com/ocornut/imgui/issues/6263#issuecomment-1479727227))
    imgui_new_frame();
    check_for_imgui_item_picker_request();
    imgui_render(_app);
    end_frame();
    dispatch_all_events(); // Must be after `imgui_render()` in order for the extra_widgets on the Views to tell us wether we are allowed to dispatch View events.
    for (auto& view : _views)
        view->on_frame_end();
    TextureLibrary_FromWebcam::instance().on_frame_end();
    webgpu_context().mainRenderPass.end();
    webgpu_context().mainRenderPass.release();

    nextTexture.release();

    wgpu::CommandBufferDescriptor cmdBufferDescriptor{};
    cmdBufferDescriptor.label   = "Command buffer";
    wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
    encoder.release();
    webgpu_context().queue.submit(command);
    command.release();
#ifndef __EMSCRIPTEN__
    webgpu_context().swapChain.present();
#endif

#ifdef WEBGPU_BACKEND_DAWN
    // Check for pending error callbacks
    webgpu_context().device.tick();
#endif
}

static void imgui_new_frame()
{
    // #if defined(COOL_VULKAN)
    //     ImGui_ImplVulkan_NewFrame();
    // #elif defined(COOL_OPENGL)
    //     ImGui_ImplOpenGL3_NewFrame();
    // #endif
    ImGui_ImplWGPU_NewFrame();
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

void AppManager::end_frame()
{
    ImGui::EndFrame();
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    {
        // HACK(ImNodes) Since nodes' workspace is not rendered during the first frame,
        // to avoid a flash we skip ImGui display altogether for the first few frames.
        // NB: go check out the other "HACK(ImNodes)"
        _frames_count++;
        if (_frames_count <= 2)
            draw_data = nullptr;
    }
    // #if defined(COOL_VULKAN)
    //     const bool can_render = draw_data && draw_data->DisplaySize.x > 0.0f && draw_data->DisplaySize.y > 0.0f;
    //     if (can_render)
    //     {
    //         window().FrameRender(draw_data);
    //     }
    //     if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //     {
    //         ImGui::UpdatePlatformWindows();
    //         ImGui::RenderPlatformWindowsDefault();
    //     }
    //     if (can_render)
    //     {
    //         window().FramePresent();
    //     }
    // #elif defined(COOL_OPENGL)
    //     ImGuiIO& io = ImGui::GetIO();
    //     glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    //     if (draw_data)
    //         ImGui_ImplOpenGL3_RenderDrawData(draw_data);
    //     if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) // NOLINT
    //     {
    //         GLFWwindow* backup_current_context = glfwGetCurrentContext(); // TODO(WebGPU) Is this still needed when not using OpenGL?
    //         ImGui::UpdatePlatformWindows();
    //         ImGui::RenderPlatformWindowsDefault();
    //         glfwMakeContextCurrent(backup_current_context);
    //     }
    //     glfwSwapBuffers(window().glfw());
    // #endif

    if (draw_data)
        ImGui_ImplWGPU_RenderDrawData(draw_data, webgpu_context().mainRenderPass);
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

void AppManager::dispatch_all_events()
{
    if (!_app.inputs_are_allowed())
        return;
    dispatch_mouse_movement();
    dispatch_mouse_click();
    dispatch_mouse_scroll();
}

void AppManager::dispatch_mouse_movement()
{
    if (ImGui::GetIO().MouseDelta == ImVec2{0.f, 0.f})
        return;

    auto const event = MouseMoveEvent<ImGuiCoordinates>{
        .position = ImGui::GetIO().MousePos,
        .delta    = ImGui::GetIO().MouseDelta,
    };
    for (auto& view : _views)
        view->dispatch_mouse_move_event(event);
}

void AppManager::dispatch_mouse_click()
{
    for (int button = 0; button < IM_ARRAYSIZE(ImGui::GetIO().MouseClicked); button++)
    {
        if (ImGui::IsMouseClicked(button))
        {
            auto const event = MouseButtonEvent<ImGuiCoordinates>{
                .position = ImGui::GetIO().MousePos,
                .button   = button,
                .action   = ButtonAction::Pressed,
            };
            for (auto& view : _views)
                view->dispatch_mouse_button_event(event);
        }
        if (ImGui::IsMouseReleased(button))
        {
            auto const event = MouseButtonEvent<ImGuiCoordinates>{
                .position = ImGui::GetIO().MousePos,
                .button   = button,
                .action   = ButtonAction::Released,
            };
            for (auto& view : _views)
                view->dispatch_mouse_button_event(event);
        }
    }
}

void AppManager::dispatch_mouse_scroll()
{
    float const scroll_x = ImGui::GetIO().MouseWheelH;
    float const scroll_y = ImGui::GetIO().MouseWheel;
    if (scroll_x == 0.f && scroll_y == 0.f)
        return;

    auto const event = MouseScrollEvent<ImGuiCoordinates>{
        .position = ImGui::GetIO().MousePos,
        .dx       = scroll_x,
        .dy       = scroll_y,
    };

    for (auto& view : _views)
        view->dispatch_mouse_scroll_event(event);
}

void AppManager::imgui_windows()
{
    Cool::DebugOptions::style_editor([&]() {
        _style_editor.imgui();
    });
}

} // namespace Cool
