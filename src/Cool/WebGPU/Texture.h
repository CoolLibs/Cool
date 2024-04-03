#pragma once
#include <img/src/Size.h>
#include "Cool/WebGPU/TextureView.h"
#include "Cool/WebGPU/WGPUUnique.h"
#include "WebGPU/webgpu.hpp"

namespace Cool {

class Texture : public WGPUUnique<wgpu::Texture> {
public:
    Texture() = default; // TODO(WebGPU) Remove ?
    explicit Texture(wgpu::TextureDescriptor const&);

    auto               imgui_texture_id() const -> ImTextureID { return entire_texture_view().handle(); }
    [[nodiscard]] auto size() const -> img::Size { return img::Size{_desc.size.width, _desc.size.height}; }

    auto entire_texture_view() const -> TextureView const&; // TODO(WebGPU) When resizing the texture, we probably need to rebuild the view ?

private:
    // TODO(ColorSpaces) Store the color space of the texture (Oklab, sRGB, etc.) and use it to check / know if we need conversions etc.
    wgpu::TextureDescriptor            _desc{};
    mutable std::optional<TextureView> _entire_texture_view{};
};

} // namespace Cool