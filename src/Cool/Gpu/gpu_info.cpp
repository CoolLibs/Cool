#include "OpenGL/gpu_info.h"
#include "gpu_info.h"

namespace Cool {

auto gpu_name() -> std::string const&
{
    static std::string const str = gpu_name_impl();
    return str;
}

auto full_gpu_info() -> std::string const&
{
    static std::string const str = full_gpu_info_impl();
    return str;
}

} // namespace Cool