
#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>
//
#include <Cool/AppManager/should_we_use_a_separate_thread_for_update.h>
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_wgpu.h>
#include <imgui/imgui.h>
#include <stdexcept>
#include "BackendContext.h"
#include "Cool/Backend/WindowConfig.h"
#include "Cool/Backend/imgui_config.h"
#include "Cool/Gpu/WebGPUContext.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Path/Path.h"

namespace Cool {

static void glfw_error_callback(int error, const char* description)
{
    Log::Debug::error_without_breakpoint("glfw", fmt::format("Error {}:\n{}", error, description));
}

#if DEBUG
static void setup_webgpu_debugging()
{
    // if constexpr (COOL_OPENGL_VERSION < 430)
    //     return;

    // int flags; // NOLINT
    // glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    // if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    // {
    //     glEnable(GL_DEBUG_OUTPUT);
    //     glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    //     glDebugMessageCallback(GLDebugCallback, nullptr);
    //     glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    // }
    // else
    // {
    //     Log::Debug::warning(
    //         "WindowFactory_ImplOpenGL::setupGLDebugging",
    //         "Couldn't setup OpenGL Debugging"
    //     );
    // }
}
#endif

static void throw_error(std::string const& error_message)
{
    // TODO(WebGPU) Log to a file too, so that we can debug when the app is shipped to end customers who don't have a debugger attached
#if DEBUG
    std::cerr << error_message << '\n';
#endif
    throw std::runtime_error{error_message};
}

static void throw_glfw_error(std::string_view message)
{
    const char* err; // NOLINT(*-init-variables)
    glfwGetError(&err);
    throw_error(fmt::format("{}:\n{}", message, err));
}

static void set_window_icon(GLFWwindow* window)
{
#ifdef COOL_APP_ICON_FILE // Don't do anything if no icon has been set
    auto icon  = img::load(Cool::Path::root() / COOL_APP_ICON_FILE, 4, false);
    auto image = GLFWimage{
        .width  = static_cast<int>(icon.width()),
        .height = static_cast<int>(icon.height()),
        .pixels = icon.data(),
    };
    glfwSetWindowIcon(window, 1, &image);
#else
    std::ignore = window;
#endif
}

void apply_config(WindowConfig const& config, Window& window)
{
    window.cap_framerate_if(config.cap_framerate_on_startup_if);
    if (config.maximize_on_startup_if)
    {
        glfwMaximizeWindow(window.glfw());
    }
    if (config.hide_on_startup_if)
    {
        window.set_visibility(false);
    }
}

// TODO(WebGPU) Disable validation layers in release

BackendContext::BackendContext(WindowConfig const& config)
{
    // WebGPU
    _wgpu.instance = wgpu::createInstance(wgpu::InstanceDescriptor{});
    if (!webgpu_context().instance)
    {
        std::cerr << "Could not initialize WebGPU!" << std::endl;
    }

    // GLFW
    glfwSetErrorCallback(&glfw_error_callback);
    if (!glfwInit())
        throw_glfw_error("GLFW initialization failed");

    // GLFW window
    glfwWindowHint(GLFW_AUTO_ICONIFY, config.auto_iconify);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _window = Window{glfwCreateWindow(config.initial_width, config.initial_height, config.title, nullptr, nullptr)};
    if (!glfw_window())
        throw_glfw_error("Window creation failed");

    set_window_icon(glfw_window());
    apply_config(config, _window);

    // WebGPU
    std::cout << "Requesting adapter..." << std::endl;
    _wgpu.surface = glfwGetWGPUSurface(_wgpu.instance, glfw_window());
    wgpu::RequestAdapterOptions adapterOpts{};
    adapterOpts.powerPreference   = wgpu::PowerPreference::HighPerformance;
    adapterOpts.compatibleSurface = _wgpu.surface;
    _wgpu.adapter                 = _wgpu.instance.requestAdapter(adapterOpts);
    std::cout << "Got adapter: " << _wgpu.adapter << std::endl;

    // TODO(WebGPU) Handle limits : https://eliemichel.github.io/LearnWebGPU/getting-started/the-device.html
    wgpu::SupportedLimits supportedLimits;
#ifdef __EMSCRIPTEN__ // TODO(WebGPU) Check if this is still relevant
    // Error in Chrome: Aborted(TODO: wgpuAdapterGetLimits unimplemented)
    // (as of September 4, 2023), so we hardcode values:
    // These work for 99.95% of clients (source: https://web3dsurvey.com/webgpu)
    supportedLimits.limits.minStorageBufferOffsetAlignment = 256;
    supportedLimits.limits.minUniformBufferOffsetAlignment = 256;
#else
    _wgpu.adapter.getLimits(&supportedLimits);
#endif

    std::cout << "Requesting device..." << std::endl;
    wgpu::RequiredLimits requiredLimits                   = wgpu::Default;
    requiredLimits.limits.maxVertexAttributes             = 4;
    requiredLimits.limits.maxVertexBuffers                = 1;
    requiredLimits.limits.maxBufferSize                   = 150000 * 100 /* sizeof(VertexAttributes) */; // TODO(WebGPU)
    requiredLimits.limits.maxVertexBufferArrayStride      = 100 /* sizeof(VertexAttributes) */;
    requiredLimits.limits.minStorageBufferOffsetAlignment = supportedLimits.limits.minStorageBufferOffsetAlignment;
    requiredLimits.limits.minUniformBufferOffsetAlignment = supportedLimits.limits.minUniformBufferOffsetAlignment;
    requiredLimits.limits.maxInterStageShaderComponents   = 8;
    requiredLimits.limits.maxBindGroups                   = 2;
    //                                    ^ This was a 1
    requiredLimits.limits.maxUniformBuffersPerShaderStage = 1;
    requiredLimits.limits.maxUniformBufferBindingSize     = 16 * 4 * sizeof(float);
    // Allow textures up to 2K
    requiredLimits.limits.maxTextureDimension1D            = 2048;
    requiredLimits.limits.maxTextureDimension2D            = 2048;
    requiredLimits.limits.maxTextureArrayLayers            = 1;
    requiredLimits.limits.maxSampledTexturesPerShaderStage = 1;
    requiredLimits.limits.maxSamplersPerShaderStage        = 1;

    wgpu::DeviceDescriptor deviceDesc;
    deviceDesc.label                 = "WebGPU Device";
    deviceDesc.requiredFeaturesCount = 0;
    deviceDesc.requiredLimits        = &requiredLimits;
    deviceDesc.defaultQueue.label    = "Default queue";
    _wgpu.device                     = _wgpu.adapter.requestDevice(deviceDesc);
    std::cout << "Got device: " << _wgpu.device << std::endl;

    // Add an error callback for more debug info
#if DEBUG
    _wgpu_error_callback = _wgpu.device.setUncapturedErrorCallback([](wgpu::ErrorType, char const* message) {
        throw_error(fmt::format("WebGPU Error:\n{}", message)); // TODO(WebGPU) Don't throw on all errors ?
    });
#endif

    _wgpu.queue = _wgpu.device.getQueue();

    {
        // Create swapchain and depth buffer for the first time
        int width, height; // NOLINT(*isolate-declaration, *init-variables)
        glfwGetFramebufferSize(glfw_window(), &width, &height);
        _wgpu.on_window_size_changed(width, height);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOther(window().glfw(), true);
    ImGui_ImplWGPU_Init(webgpu_context().device, 3, webgpu_context().swapChainFormat, webgpu_context().depthTextureFormat);
    imgui_config();
}

BackendContext::~BackendContext()
{
    ImGui_ImplWGPU_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window().glfw());
    glfwTerminate();
}

} // namespace Cool