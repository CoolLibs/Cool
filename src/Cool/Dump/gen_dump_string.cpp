#include "gen_dump_string.h"
#include <Cool/Gpu/gpu_info.h>
#include <Cool/Path/Path.h>
#include <os_name/os_name.hpp>
#include <string>
#include "gpu_api_version.h"

namespace Cool {

auto gen_dump_string() -> std::string
{
    return fmt::format(
        R"STR(OS                : {}
{}
GPU API           : {})STR",
        os_name(),
        full_gpu_info(),
        gpu_api_version()
    );
}

} // namespace Cool