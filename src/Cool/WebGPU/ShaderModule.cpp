#include "ShaderModule.h"
#include "Cool/WebGPU/WebGPUContext.hpp"

namespace Cool {

static auto make_shader_module(ShaderModule_CreationArgs const& args) -> wgpu::ShaderModule
{
    wgpu::ShaderModuleDescriptor shaderDesc;
    shaderDesc.label = args.label.data();
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
    shaderCodeDesc.code = args.code.data();

    return webgpu_context().device.createShaderModule(shaderDesc); // TODO(WebGPU) setLabel() when in debug, and get compilation info
}

ShaderModule::ShaderModule(ShaderModule_CreationArgs const& args)
    : WGPUUnique<wgpu::ShaderModule>{make_shader_module(args)}
{}

} // namespace Cool