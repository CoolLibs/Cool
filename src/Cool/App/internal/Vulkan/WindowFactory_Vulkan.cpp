#include "WindowFactory_Vulkan.h"
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include "../VulkanCheckResult.h"

namespace Cool {

void WindowFactory_Vulkan::initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

WindowFactory_Vulkan::WindowFactory_Vulkan()
    : _vku_framework{""}
{
    if (!_vku_framework.ok()) {
        std::cout << "Framework creation failed" << std::endl;
        exit(1);
    }
}

WindowFactory_Vulkan::~WindowFactory_Vulkan()
{
    _vku_framework.device().waitIdle();
    for (auto& window : _windows) {
        glfwDestroyWindow(window.glfw());
    }
    glfwTerminate();
}

Cool::Window& WindowFactory_Vulkan::make_window(const char* title, int width, int height)
{
    GLFWwindow* glfw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    auto&       window      = _windows.emplace_back(glfw_window, _vku_framework);
    setup_imgui(window);
    return window;
}

void Cool::WindowFactory_Vulkan::setup_imgui(Window& window)
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

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(window.glfw(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance                  = _vku_framework.instance();
    init_info.PhysicalDevice            = _vku_framework.physicalDevice();
    init_info.Device                    = _vku_framework.device();
    init_info.QueueFamily               = _vku_framework.graphicsQueueFamilyIndex();
    init_info.Queue                     = _vku_framework.graphicsQueue();
    init_info.PipelineCache             = _vku_framework.pipelineCache();
    init_info.DescriptorPool            = _vku_framework.descriptorPool();
    init_info.Allocator                 = nullptr;
    init_info.MinImageCount             = 2;
    init_info.ImageCount                = window.vku().imageViews().size();
    init_info.CheckVkResultFn           = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, window.vku().renderPass());

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

    // Upload Fonts
    {
        // Use any command queue
        VkCommandPool   command_pool   = window.vku().commandPool(); //window._vulkan_window_state.g_MainWindowData.Frames[window._vulkan_window_state.g_MainWindowData.FrameIndex].CommandPool;
        VkCommandBuffer command_buffer = window.vku().commandBuffers()[0].get();

        VkResult err = vkResetCommandPool(_vku_framework.device(), command_pool, 0);
        check_vk_result(err);
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(command_buffer, &begin_info);
        check_vk_result(err);

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info       = {};
        end_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers    = &command_buffer;
        err                         = vkEndCommandBuffer(command_buffer);
        check_vk_result(err);
        err = vkQueueSubmit(_vku_framework.graphicsQueue(), 1, &end_info, VK_NULL_HANDLE);
        check_vk_result(err);

        err = vkDeviceWaitIdle(_vku_framework.device());
        check_vk_result(err);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
}

} // namespace Cool