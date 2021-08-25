#pragma once
#if defined(__COOL_APP_VULKAN)

#include <Cool/Gpu/RenderTargetInfo.h>
#include "ShaderModule.h"

namespace Cool::Vulkan {

class FullscreenPipeline {
public:
    FullscreenPipeline(std::string_view fragment_shader_path);

    void rebuild_for_render_target(const RenderTargetInfo& render_target_info);

    void draw(vk::CommandBuffer cb);

    vk::PipelineLayout layout() const { return *_pipeline_layout; }

public:
    ShaderModule             _fragment_shader_module;
    ShaderModule             _vertex_shader_module;
    vk::UniquePipeline       _pipeline;
    vk::UniquePipelineLayout _pipeline_layout;
};

} // namespace Cool::Vulkan

#endif