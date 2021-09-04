#if defined(__COOL_APP_VULKAN)

#include "WindowFactory_Vulkan.h"
#include <Cool/Gpu/Vulkan/Context.h>
#include <Cool/Gpu/Vulkan/internal/check_result.h>
#include <Cool/Log/ToUser.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/imgui.h>
#include <vulkan/vulkan.h>

namespace Cool {

WindowFactory_Vulkan::WindowFactory_Vulkan()
{
    Log::info("[Gpu] Using Vulkan");
}

void WindowFactory_Vulkan::shut_down(WindowManager& window_manager)
{
    Vulkan::context().destroy0(); // TODO is this the proper place to do it ?
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    window_manager.windows().clear();
    Vulkan::context().destroy1(); // TODO is this the proper place to do it ?
    glfwTerminate();
}

void WindowFactory_Vulkan::setup_main_window(Window_Vulkan& window)
{
    setup_imgui(window);
}

void WindowFactory_Vulkan::setup_secondary_window(Window_Vulkan& window, WindowManager& window_manager)
{
}

// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
static void SetupVulkanWindow(VulkanWindowState& vulkan_window_state, VkSurfaceKHR surface, int width, int height)
{
    ImGui_ImplVulkanH_Window* wd = &vulkan_window_state.g_MainWindowData;
    wd->Surface                  = surface;

    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(Vulkan::context().g_PhysicalDevice, Vulkan::context().g_QueueFamily, wd->Surface, &res);
    if (res != VK_TRUE) {
        fprintf(stderr, "Error no WSI support on physical device 0\n");
        exit(-1);
    }

    // Select Surface Format
    const VkFormat        requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
    const VkColorSpaceKHR requestSurfaceColorSpace    = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    wd->SurfaceFormat                                 = ImGui_ImplVulkanH_SelectSurfaceFormat(Vulkan::context().g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
}

Window_Vulkan& WindowFactory_Vulkan::make_window(const WindowCreationParams& params, WindowManager& window_manager)
{
    auto& vk_context = Vulkan::context();
    // Window flags
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Create window
    window_manager.windows().push_back(Window_Vulkan{glfwCreateWindow(params.width, params.height, params.name, NULL, NULL)});
    Window_Vulkan& window = window_manager.windows().back();
    if (!window.glfw()) {
        const char* errorDescription;
        glfwGetError(&errorDescription);
        Log::error("[Glfw] Window creation failed :\n{}", errorDescription);
    }
    // Create Window Surface
    VkSurfaceKHR surface;
    VkResult     err = glfwCreateWindowSurface(vk_context.g_Instance, window.glfw(), vk_context.g_Allocator, &surface);
    Vulkan::check_result(err);

    // Create Framebuffers
    int w, h;
    glfwGetFramebufferSize(window.glfw(), &w, &h);
    SetupVulkanWindow(window._vulkan_window_state, surface, w, h);

    // Check present mode's availability
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        Vulkan::context().g_PhysicalDevice,
        surface,
        &presentModeCount,
        nullptr);

    std::vector<VkPresentModeKHR> present_modes{presentModeCount};
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        Vulkan::context().g_PhysicalDevice,
        surface,
        &presentModeCount,
        present_modes.data());

    window._present_mode_mailbox_is_avaible =
        std::find(present_modes.begin(), present_modes.end(), VK_PRESENT_MODE_MAILBOX_KHR) != present_modes.end();

    window.cap_framerate(params.cap_framerate);

    return window;
}

void WindowFactory_Vulkan::setup_imgui(Window_Vulkan& window)
{
    ImGui_ImplGlfw_InitForVulkan(window.glfw(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance                  = Vulkan::context().g_Instance;
    init_info.PhysicalDevice            = Vulkan::context().g_PhysicalDevice;
    init_info.Device                    = Vulkan::context().g_Device;
    init_info.QueueFamily               = Vulkan::context().g_QueueFamily;
    init_info.Queue                     = Vulkan::context().g_Queue;
    init_info.PipelineCache             = Vulkan::context().g_PipelineCache;
    init_info.DescriptorPool            = Vulkan::context().g_DescriptorPool;
    init_info.Allocator                 = Vulkan::context().g_Allocator;
    init_info.MinImageCount             = window._vulkan_window_state.g_MinImageCount;
    init_info.ImageCount                = window._vulkan_window_state.g_MainWindowData.ImageCount;
    init_info.CheckVkResultFn           = Vulkan::check_result;
    ImGui_ImplVulkan_Init(&init_info, window._vulkan_window_state.g_MainWindowData.RenderPass);

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
        VkCommandPool   command_pool   = window._vulkan_window_state.g_MainWindowData.Frames[window._vulkan_window_state.g_MainWindowData.FrameIndex].CommandPool;
        VkCommandBuffer command_buffer = window._vulkan_window_state.g_MainWindowData.Frames[window._vulkan_window_state.g_MainWindowData.FrameIndex].CommandBuffer;

        VkResult err = vkResetCommandPool(Vulkan::context().g_Device, command_pool, 0);
        Vulkan::check_result(err);
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(command_buffer, &begin_info);
        Vulkan::check_result(err);

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info       = {};
        end_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers    = &command_buffer;
        err                         = vkEndCommandBuffer(command_buffer);
        Vulkan::check_result(err);
        err = vkQueueSubmit(Vulkan::context().g_Queue, 1, &end_info, VK_NULL_HANDLE);
        Vulkan::check_result(err);

        err = vkDeviceWaitIdle(Vulkan::context().g_Device);
        Vulkan::check_result(err);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
}

} // namespace Cool

#endif