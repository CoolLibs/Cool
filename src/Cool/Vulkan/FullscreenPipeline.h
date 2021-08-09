#pragma once

#include <Cool/Gpu/RenderTargetInfo.h>
#include "ShaderModule.h"

namespace Cool::Vulkan {

class FullscreenPipeline {
public:
    FullscreenPipeline(std::string_view fragment_shader_path);

    vk::UniquePipeline make_unique(const RenderTargetInfo& render_target_info);

    void draw(vk::CommandBuffer cb, vk::UniquePipeline& pipeline);

private:
    ShaderModule _fragment_shader_module;
    ShaderModule _vertex_shader_module;
};

} // namespace Cool::Vulkan