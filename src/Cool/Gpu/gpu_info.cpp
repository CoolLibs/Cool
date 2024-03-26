#include "gpu_info.h"

namespace Cool {

auto full_gpu_info() -> std::string
{
    // TODO(WebGPU)
    return "WebGPU";
    //     const char* vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    //     const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    //     const char* version  = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    //     return fmt::format(R"STR(GPU vendor        : {}
    // GPU model         : {}
    // GPU version       : {})STR",
    //                        vendor, renderer, version);
}

auto gpu_name() -> std::string
{
    return "WebGPU"; // reinterpret_cast<const char*>(glGetString(GL_RENDERER)); // TODO(WebGPU)
}

} // namespace Cool