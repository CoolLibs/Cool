#pragma once
#include "BindGroup.hpp"
#include "BindGroupLayout.hpp"
#include "Buffer.h"
#include "Texture.h"
#include "TextureSampler.hpp"
#include "TextureSamplerDescriptor.hpp"

namespace Cool {

class BindGroupBuilder {
public:
    auto read_texture(int binding, Texture const&) -> BindGroupBuilder&;
    auto sampler(int binding, TextureSampler const&) -> BindGroupBuilder&;
    auto sampler(int binding, TextureSamplerDescriptor const&) -> BindGroupBuilder&;
    auto write_texture_2D(int binding) -> BindGroupBuilder&;
    auto buffer(int binding, Buffer const&) -> BindGroupBuilder&;

    auto build(BindGroupLayout const& layout) const -> BindGroup;

private:
    std::vector<wgpu::BindGroupEntry> _entries{};
};

} // namespace Cool