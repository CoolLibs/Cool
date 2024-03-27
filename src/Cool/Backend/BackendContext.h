#pragma once
#include "Cool/Gpu/WebGPUContext.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include "WindowConfig.h"

namespace Cool {

class BackendContext { // NOLINT(*special-member-functions)
public:
    // This is a singleton, use backend_context() to get the global instance.
    explicit BackendContext(WindowConfig const& config);
    ~BackendContext();

    auto webgpu_context() -> WebGPUContext& { return _wgpu; }
    auto window() -> Window& { return _window; }

private:
    WebGPUContext _wgpu{};
    Window        _window{};

#if DEBUG
    std::unique_ptr<wgpu::ErrorCallback> _wgpu_error_callback{}; // Keep the error callback alive
#endif
};

inline auto backend_context() -> std::optional<BackendContext>&
{
    static auto instance = std::optional<BackendContext>{};
    return instance;
}

inline void init_backend_context(WindowConfig const& config)
{
    backend_context().emplace(config);
}

} // namespace Cool