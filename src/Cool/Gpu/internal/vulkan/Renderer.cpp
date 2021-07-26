// #ifdef __COOL_APP_VULKAN

//     #include "../../Renderer.h"
//     #include <Cool/App/Window.h>

// namespace Cool {

// Window* Renderer::_window;

// void Renderer::clear_background(const glm::vec3& clear_color)
// {
//     // auto& wd                       = _window->_vulkan_window_state.g_MainWindowData;
//     // wd.ClearValue.color.float32[0] = clear_color.r;
//     // wd.ClearValue.color.float32[1] = clear_color.g;
//     // wd.ClearValue.color.float32[2] = clear_color.b;
//     // wd.ClearValue.color.float32[3] = 1.f;
//     // TODO call the render pass immediately
// }

// void Renderer::set_render_target(Window& window)
// {
//     _window = &window;
// }

// // VkDevice Renderer::device()
// // {
// //     // assert(_window != nullptr);
// //     // return _window->_vulkan_context.g_Device;
// // }

// } // namespace Cool

// #endif