#include "ShaderModule.h"
#include <webgpu/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

static auto make(std::string_view wgsl_source_code) -> wgpu::ShaderModule
{
    wgpu::ShaderModuleDescriptor shaderDesc;
#ifdef WEBGPU_BACKEND_WGPU
    shaderDesc.hintCount = 0;
    shaderDesc.hints     = nullptr;
#endif

    // Use the extension mechanism to load a WGSL shader source code
    wgpu::ShaderModuleWGSLDescriptor shaderCodeDesc;
    // Set the chained struct's header
    shaderCodeDesc.chain.next  = nullptr;
    shaderCodeDesc.chain.sType = wgpu::SType::ShaderModuleWGSLDescriptor;
    // Connect the chain
    shaderDesc.nextInChain = &shaderCodeDesc.chain;

    // Setup the actual payload of the shader code descriptor
    shaderCodeDesc.code = wgsl_source_code.data();

    return webgpu_context().device.createShaderModule(shaderDesc);
}

ShaderModule::ShaderModule(std::string_view wgsl_source_code)
    : _shader_module{make(wgsl_source_code)}
{}

} // namespace Cool