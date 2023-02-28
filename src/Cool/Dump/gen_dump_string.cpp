#include "gen_dump_string.h"
#include <Cool/Gpu/gpu_info.h>
#include <os_name/os_name.hpp>
#include <string>

namespace Cool {

auto gen_dump_string() -> std::string
{
    return fmt::format(
        R"STR(---INFO---
OS     : {}
{}
)STR",
        os_name(),
        full_gpu_info()
    );
}

} // namespace Cool