#if defined(COOL_VULKAN)

#include "../FullscreenPipeline.h"
#include <Cool/File/File.h>
#include <Cool/Path/Path.h>
#include "Context.h"

namespace Cool::Vulkan {

FullscreenPipeline::FullscreenPipeline(std::string_view fragment_shader_source_code, std::string_view name)
    : _vertex_shader_module{{File::to_string(Path::cool_res() + "/shaders/fullscreen.vert"), Cool::ShaderKind::Vertex}}
    , _fragment_shader_module{{std::string{fragment_shader_source_code}, Cool::ShaderKind::Fragment}}
{
}

void FullscreenPipeline::shut_down()
{
    // vertex_module().reset(); // TODO(JF)
}

void FullscreenPipeline::rebuild_for_render_target(const RenderTargetInfo& render_target_info)
{
    auto& device = Vulkan::context().g_Device;
    vkDeviceWaitIdle(device);

    // Make a default pipeline layout. This shows how pointers
    // to resources are layed out.
    vku::PipelineLayoutMaker plm{};
    plm.pushConstantRange(vk::ShaderStageFlagBits::eFragment, 0, 96); // TODO(JF) push constants shouldn't be hardcoded
    _pipeline_layout = plm.createUnique(device);

    // Make a pipeline to use the vertex format and shaders.
    vku::PipelineMaker pm{
        render_target_info.viewport.bottom_left_corner.x,
        render_target_info.viewport.bottom_left_corner.y,
        render_target_info.viewport.size.width(),
        render_target_info.viewport.size.height()};
    pm.shader(vk::ShaderStageFlagBits::eVertex, _vertex_shader_module.vku());
    pm.shader(vk::ShaderStageFlagBits::eFragment, _fragment_shader_module.vku());
    pm.blendBegin(VK_TRUE);

    // Create a pipeline using a renderPass built for our window.
    auto renderPass = render_target_info.render_pass;
    auto cache      = Vulkan::context().g_PipelineCache;
    _pipeline       = pm.createUnique(device, cache, *_pipeline_layout, renderPass);
}

void FullscreenPipeline::draw(vk::CommandBuffer cb)
{
    cb.bindPipeline(vk::PipelineBindPoint::eGraphics, *_pipeline);
    cb.draw(3, 1, 0, 0);
}

} // namespace Cool::Vulkan

#endif