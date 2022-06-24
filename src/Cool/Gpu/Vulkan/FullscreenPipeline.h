#pragma once
#if defined(COOL_VULKAN)

#include <Cool/Gpu/RenderTargetInfo.h>
#include "ShaderModule.h"

namespace Cool::Vulkan {

class FullscreenPipeline {
public:
    /**
     * @param name The name that will be displayed in the error message if the compilation fails
     */
    FullscreenPipeline(std::string_view fragment_shader_source_code, std::string_view name);

    static void shut_down();

    void rebuild_for_render_target(const RenderTargetInfo& render_target_info);

    void draw(vk::CommandBuffer cb);

    vk::PipelineLayout layout() const { return *_pipeline_layout; }

public:
    ShaderModule             _vertex_shader_module;
    ShaderModule             _fragment_shader_module;
    vk::UniquePipeline       _pipeline;
    vk::UniquePipelineLayout _pipeline_layout;
};

} // namespace Cool::Vulkan

#endif