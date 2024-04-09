#include "ShaderModule.h"
#include <ShaderTranspiler/ShaderTranspiler.hpp>
#include "Cool/Gpu/WebGPUContext.h"
#include "preprocess_shader_source.h"

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
    shaderCodeDesc.code = args.wgsl_code.data();

    return webgpu_context().device.createShaderModule(shaderDesc); // TODO(WebGPU) setLabel() when in debug, and get compilation info
}

ShaderModule::ShaderModule(ShaderModule_CreationArgs const& args)
    : WGPUUnique<wgpu::ShaderModule>{make_shader_module(args)}
{}

auto transpile_glsl_to_wgsl(std::string_view glsl_code) -> std::string
{
    auto const preprocessed = preprocess_shader_source(glsl_code);
    // TODO(WebGPU) Handle error properly
    if (!preprocessed)
    {
        std::cerr << preprocessed.error() << '\n';
        throw preprocessed.error();
    }

    // ImGui::SetClipboardText(preprocessed.value().c_str());
    shadert::ShaderTranspiler s;

    shadert::MemoryCompileTask task{preprocessed.value(), shadert::ShaderStage::Fragment};

    shadert::Options opt;
    opt.enableInclude   = false;
    opt.debug           = false;
    opt.entryPoint      = "main";
    opt.preambleContent = "";
    // opt.uniformBufferSettings
    opt.mobile  = false; // used for OpenGL ES or Metal iOS
    opt.version = 13;    // stores the major and minor version, for Vulkan 1.5 use 15

    try
    {
        shadert::CompileResult result = s.CompileTo(task, shadert::TargetAPI::WGSL, opt);
        std::cout << result.data.sourceData << '\n';
        return result.data.sourceData;
    }
    catch (std::exception& e)
    {
        // library will throw on errors
        // TODO(WebGPU) handle error properly
        std::cerr << e.what() << '\n';
        return "";
    }
}

} // namespace Cool