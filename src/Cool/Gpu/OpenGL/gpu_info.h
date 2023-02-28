#pragma once

#if defined(COOL_OPENGL)
namespace Cool {
auto gpu_name_impl() -> std::string;
auto full_gpu_info_impl() -> std::string;
} // namespace Cool
#endif