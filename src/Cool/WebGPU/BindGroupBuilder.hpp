#pragma once
#include "BindGroup.hpp"
#include "BindGroupLayout.hpp"
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
    auto read_buffer(int binding) -> BindGroupBuilder&;
    auto write_buffer(int binding) -> BindGroupBuilder&;
    auto read_write_buffer(int binding) -> BindGroupBuilder&;

    auto build(BindGroupLayout const& layout) const -> BindGroup;

private:
    std::vector<wgpu::BindGroupEntry> _entries{};
};

} // namespace Cool