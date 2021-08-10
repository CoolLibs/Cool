#include "WindowFactory_Base.h"
#include <Cool/Log/ToUser.h>
#include <Cool/Vulkan/Context.h>
#include <glfw/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/imgui.h>

namespace Cool {

WindowFactory_Base::WindowFactory_Base()
{
    initializeGLFW();
    setup_imgui();
}

void WindowFactory_Base::initializeGLFW()
{
    glfwSetErrorCallback(WindowFactory_Base::GlfwErrorCallback);
    if (!glfwInit()) {
        const char* errorDescription;
        glfwGetError(&errorDescription);
        Log::error("Glfw initialization failed :\n{}", errorDescription);
    }
}

void WindowFactory_Base::GlfwErrorCallback(int error, const char* description)
{
    Log::error("[Glfw] Error {} :\n{}", error, description);
}

void WindowFactory_Base::setup_imgui()
{
    // Setup context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup style
    ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

} // namespace Cool