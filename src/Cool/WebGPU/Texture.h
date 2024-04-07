#pragma once
#include <img/src/Size.h>
#include "Cool/WebGPU/TextureView.h"
#include "Cool/WebGPU/WGPUUnique.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

class Texture : public WGPUUnique<wgpu::Texture> {
public:
    Texture() = default; // TODO(WebGPU) Remove ?
    explicit Texture(wgpu::TextureDescriptor const&);
    void set_image(uint32_t color_components_count, std::span<uint8_t const> data);

    auto               imgui_texture_id() const -> ImTextureID { return entire_texture_view().handle(); }
    [[nodiscard]] auto size() const -> img::Size { return img::Size{_desc.size.width, _desc.size.height}; }

    auto entire_texture_view() const -> TextureView const&; // TODO(WebGPU) When resizing the texture, we probably need to rebuild the view ?
    auto descriptor() const -> wgpu::TextureDescriptor { return _desc; }

private:
    // TODO(ColorSpaces) Store the color space of the texture (Oklab, sRGB, etc.) and use it to check / know if we need conversions etc.
    wgpu::TextureDescriptor            _desc{};
    mutable std::optional<TextureView> _entire_texture_view{};
};

auto load_texture(std::filesystem::path const& path) -> Texture;
auto texture_from_pixels(img::Size size, std::span<uint8_t const> data) -> Texture;

} // namespace Cool