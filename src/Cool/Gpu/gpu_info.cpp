#include "gpu_info.h"
#include "Cool/Gpu/WebGPUContext.h"
#include "magic_enum/include/magic_enum/magic_enum.hpp"
#include "webgpu/webgpu.hpp"

namespace Cool {

auto full_gpu_info() -> std::string
{
    wgpu::AdapterProperties props;
    webgpu_context().adapter.getProperties(&props);
    return fmt::format(
        R"STR(GPU               : {} ({} {})
GPU Type          : {}
GPU Backend       : {}
GPU Driver        : {})STR",
        props.name,
        props.vendorName,
        props.architecture,
        magic_enum::enum_name(props.adapterType),
        magic_enum::enum_name(props.backendType),
        props.driverDescription
    );
}

} // namespace Cool