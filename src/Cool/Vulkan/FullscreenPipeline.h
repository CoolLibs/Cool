#pragma once

#include <Cool/Gpu/RenderTargetInfo.h>
#include "ShaderModule.h"

namespace Cool::Vulkan {

class FullscreenPipeline {
public:
    FullscreenPipeline(std::string_view fragment_shader_path);

    void rebuild_for_render_target(const RenderTargetInfo& render_target_info);

    void draw(vk::CommandBuffer cb);

private:
    ShaderModule       _fragment_shader_module;
    ShaderModule       _vertex_shader_module;
    vk::UniquePipeline _pipeline;
};

} // namespace Cool::Vulkan