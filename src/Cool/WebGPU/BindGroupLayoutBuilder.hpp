#pragma once
#include "BindGroupLayout.hpp"

namespace Cool {

class BindGroupLayoutBuilder {
public:
    explicit BindGroupLayoutBuilder(std::optional<wgpu::ShaderStageFlags> default_visibility = {})
        : _default_visibility{default_visibility}
    {}

    auto read_texture_1D(int binding, std::optional<wgpu::ShaderStageFlags> visibility = {}) -> BindGroupLayoutBuilder&;
    auto read_texture_2D(int binding, std::optional<wgpu::ShaderStageFlags> visibility = {}) -> BindGroupLayoutBuilder&;
    auto sampler(int binding, std::optional<wgpu::ShaderStageFlags> visibility = {}) -> BindGroupLayoutBuilder&;
    auto write_texture_2D(int binding, std::optional<wgpu::ShaderStageFlags> visibility = {}) -> BindGroupLayoutBuilder&;
    auto read_buffer(int binding, std::optional<wgpu::ShaderStageFlags> visibility = {}) -> BindGroupLayoutBuilder&;
    auto write_buffer(int binding, std::optional<wgpu::ShaderStageFlags> visibility = {}) -> BindGroupLayoutBuilder&;
    auto read_write_buffer(int binding, std::optional<wgpu::ShaderStageFlags> visibility = {}) -> BindGroupLayoutBuilder&;

    auto build() const -> BindGroupLayout;

private:
    std::vector<wgpu::BindGroupLayoutEntry> _entries{};
    std::optional<wgpu::ShaderStageFlags>   _default_visibility{};
};

} // namespace Cool