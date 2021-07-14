#include "../WindowFactory.h"
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/imgui.h>
#ifdef __COOL_APP_VULKAN
    #include <imgui/backends/imgui_impl_vulkan.h>
    #include <vulkan/vulkan.h>
    #include "VulkanCheckResult.h"
#endif
#ifdef __COOL_APP_OPENGL
    #include <imgui/backends/imgui_impl_opengl3.h>
    #include "GLDebugCallback.h"
#endif

namespace Cool {

#ifdef DEBUG
bool WindowFactory::s_bInitialized = false;
#endif

#ifdef __COOL_APP_VULKAN
// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
static void SetupVulkanWindow(const VulkanContext& vulkan_context, VulkanWindowState& vulkan_window_state, VkSurfaceKHR surface, int width, int height)
{
    ImGui_ImplVulkanH_Window* wd = &vulkan_window_state.g_MainWindowData;
    wd->Surface                  = surface;

    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(vulkan_context.g_PhysicalDevice, vulkan_context.g_QueueFamily, wd->Surface, &res);
    if (res != VK_TRUE) {
        fprintf(stderr, "Error no WSI support on physical device 0\n");
        exit(-1);
    }

    // Select Surface Format
    const VkFormat        requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
    const VkColorSpaceKHR requestSurfaceColorSpace    = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    wd->SurfaceFormat                                 = ImGui_ImplVulkanH_SelectSurfaceFormat(vulkan_context.g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

    // Select Present Mode
    #ifdef IMGUI_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};
    #else
    VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
    #endif
    wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(vulkan_context.g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
    //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

    // Create SwapChain, RenderPass, Framebuffer, etc.
    IM_ASSERT(vulkan_window_state.g_MinImageCount >= 2);
    ImGui_ImplVulkanH_CreateOrResizeWindow(vulkan_context.g_Instance, vulkan_context.g_PhysicalDevice, vulkan_context.g_Device, wd, vulkan_context.g_QueueFamily, vulkan_context.g_Allocator, width, height, vulkan_window_state.g_MinImageCount);
}
#endif

WindowFactory::WindowFactory(int openGLMajorVersion, int openGLMinorVersion)
    : m_openGLMajorVersion(openGLMajorVersion), m_openGLMinorVersion(openGLMinorVersion), m_openGLVersion(openGLMajorVersion * 100 + openGLMinorVersion * 10)
{
#ifdef DEBUG
    assert(openGLMajorVersion >= 3);
    assert(openGLMinorVersion >= 3);
    if (s_bInitialized)
        Log::error("You are creating a WindowFactory twice !");
    s_bInitialized = true;
#endif
    // Init
    initializeGLFW();
#ifdef __COOL_APP_VULKAN
    // Setup Vulkan
    if (!glfwVulkanSupported()) {
        Log::ToUser::error("GLFW : Vulkan Not Supported");
    }
    uint32_t     extensions_count = 0;
    const char** extensions       = glfwGetRequiredInstanceExtensions(&extensions_count);
    _vulkan_contexts.emplace_back(extensions, extensions_count);
#endif
}

WindowFactory::~WindowFactory()
{
#ifdef __COOL_APP_VULKAN
    for (auto& context : _vulkan_contexts)
        context.destroy0();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    for (auto& window : _windows)
        window.destroy();
    for (auto& context : _vulkan_contexts)
        context.destroy1();
    glfwTerminate();
#endif
#ifdef __COOL_APP_OPENGL
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    for (auto& window : _windows)
        window.destroy();
    glfwTerminate();
#endif
}

void WindowFactory::initializeGLFW()
{
    glfwSetErrorCallback(WindowFactory::GlfwErrorCallback);
    if (!glfwInit()) {
        const char* errorDescription;
        glfwGetError(&errorDescription);
        Log::error("Glfw initialization failed :\n{}", errorDescription);
    }
}

void WindowFactory::GlfwErrorCallback(int error, const char* description)
{
    Log::error("[Glfw] Error {} :\n{}", error, description);
}

#ifdef __COOL_APP_VULKAN
Window& WindowFactory::create(const char* name, int width, int height)
{
    VulkanContext& vk_context = _vulkan_contexts[0];
    // Window flags
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Create window
    _windows.emplace_back(
        glfwCreateWindow(width, height, name, NULL, NULL),
        vk_context);
    Window& window = _windows.back();
    if (!window.get()) {
        const char* errorDescription;
        glfwGetError(&errorDescription);
        Log::error("[Glfw] Window creation failed :\n{}", errorDescription);
    }
    // TODO remove / adapt ?
    // window.makeCurrent();
    // window.enableVSync();
    // Create Window Surface
    VkSurfaceKHR surface;
    VkResult     err = glfwCreateWindowSurface(vk_context.g_Instance, window.get(), vk_context.g_Allocator, &surface);
    check_vk_result(err);

    // Create Framebuffers
    int w, h;
    glfwGetFramebufferSize(window.get(), &w, &h);
    SetupVulkanWindow(window._vulkan_context, window._vulkan_window_state, surface, w, h);
    setupImGui(window);
    return window;
}
#endif
#ifdef __COOL_APP_OPENGL
Window& WindowFactory::create(const char* name, int width, int height, GLFWwindow* windowToShareContextWith)
{
    // Window flags
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_openGLMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_openGLMinorVersion);
    #ifdef DEBUG
    if (m_openGLVersion >= 430)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    #endif
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    // Create window
    _windows.emplace_back(
        glfwCreateWindow(width, height, name, NULL, windowToShareContextWith));
    Window& window = _windows.back();
    if (!window.get()) {
        const char* errorDescription;
        glfwGetError(&errorDescription);
        Log::error("[Glfw] Window or OpenGL context creation failed :\n{}", errorDescription);
    }
    window.makeCurrent();
    window.enableVSync();
    // Load Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        Log::error("Failed to initialize Glad");
    //
    setupGLDebugging();
    setupImGui(window);
    return window;
}

void WindowFactory::setupGLDebugging()
{
    #ifdef DEBUG
    if (m_openGLVersion >= 430) {
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        else {
            Log::warn("Couldn't setup OpenGL Debugging");
        }
    }
    #endif
}
#endif

void WindowFactory::setupImGui(Window& window)
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
#ifdef __COOL_APP_VULKAN
    ImGui_ImplGlfw_InitForVulkan(window.get(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance                  = window._vulkan_context.g_Instance;
    init_info.PhysicalDevice            = window._vulkan_context.g_PhysicalDevice;
    init_info.Device                    = window._vulkan_context.g_Device;
    init_info.QueueFamily               = window._vulkan_context.g_QueueFamily;
    init_info.Queue                     = window._vulkan_context.g_Queue;
    init_info.PipelineCache             = window._vulkan_context.g_PipelineCache;
    init_info.DescriptorPool            = window._vulkan_context.g_DescriptorPool;
    init_info.Allocator                 = window._vulkan_context.g_Allocator;
    init_info.MinImageCount             = window._vulkan_window_state.g_MinImageCount;
    init_info.ImageCount                = window._vulkan_window_state.g_MainWindowData.ImageCount;
    init_info.CheckVkResultFn           = check_vk_result;
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

        VkResult err = vkResetCommandPool(window._vulkan_context.g_Device, command_pool, 0);
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
        err = vkQueueSubmit(window._vulkan_context.g_Queue, 1, &end_info, VK_NULL_HANDLE);
        check_vk_result(err);

        err = vkDeviceWaitIdle(window._vulkan_context.g_Device);
        check_vk_result(err);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
#endif
#ifdef __COOL_APP_OPENGL
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    std::string glslVersion = "#version " + std::to_string(m_openGLVersion);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
#endif
}

} // namespace Cool