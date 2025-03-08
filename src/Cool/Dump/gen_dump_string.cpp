#include "gen_dump_string.h"
#include <Cool/Gpu/gpu_info.h>
#include <Cool/Path/Path.h>
#include <os_name/os_name.hpp>
#include <string>
#include "app_version.hpp"
#include "gpu_api_version.h"

namespace Cool {

auto gen_dump_string() -> std::string
{
    return fmt::format(
        R"STR(---INFO---
{} version : {}
OS                : {}
GPU API           : {}
----------)STR",
        COOL_APP_NAME, app_version(),
        os_name(),
        // full_gpu_info(),// TODO(WebGPU) Check if we can access the info even if we failed to init WebGPU
        gpu_api_version()
    );
}

} // namespace Cool