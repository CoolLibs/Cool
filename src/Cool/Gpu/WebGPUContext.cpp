#include "WebGPUContext.h"
#include "Cool/Backend/BackendContext.h"

namespace Cool {

auto webgpu_context() -> WebGPUContext&
{
    return backend_context()->webgpu_context();
}

} // namespace Cool