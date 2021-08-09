#include "../FullscreenPipeline.h"
#include <Cool/File/File.h>
#include "../Context.h"

namespace Cool::Vulkan {

FullscreenPipeline::FullscreenPipeline(std::string_view fragment_shader_path)
    : _vertex_shader_module{File::root_dir() + "/Cool/res/shaders/fullscreen.vert", Cool::Gpu::ShaderKind::Vertex}
    , _fragment_shader_module{fragment_shader_path, Cool::Gpu::ShaderKind::Fragment}
{
}

vk::UniquePipeline FullscreenPipeline::make_unique(const RenderTargetInfo& render_target_info)
{
    auto& device = Vulkan::context().g_Device;
    vkDeviceWaitIdle(device);

    // Make a default pipeline layout. This shows how pointers
    // to resources are layed out.
    vku::PipelineLayoutMaker plm{};
    auto                     pipelineLayout_ = plm.createUnique(device);

    // Make a pipeline to use the vertex format and shaders.
    vku::PipelineMaker pm{render_target_info.width, render_target_info.height};
    pm.shader(vk::ShaderStageFlagBits::eVertex, _vertex_shader_module.vku());
    pm.shader(vk::ShaderStageFlagBits::eFragment, _fragment_shader_module.vku());

    // Create a pipeline using a renderPass built for our window.
    auto renderPass = render_target_info.render_pass;
    auto cache      = Vulkan::context().g_PipelineCache;

    return pm.createUnique(device, cache, *pipelineLayout_, renderPass);
}

void FullscreenPipeline::draw(vk::CommandBuffer cb, vk::UniquePipeline& pipeline)
{
    cb.bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline);
    cb.draw(3, 1, 0, 0);
}

} // namespace Cool::Vulkan