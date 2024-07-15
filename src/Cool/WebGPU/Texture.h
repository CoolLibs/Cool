#pragma once
#include <img/src/Size.h>
#include <cstdint>
#include "Cool/ColorSpaces/AlphaSpace.h"
#include "Cool/WebGPU/TextureView.h"
#include "Cool/WebGPU/WGPUUnique.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

class Texture : public WGPUUnique<wgpu::Texture> {
public:
    Texture() = default; // TODO(WebGPU) Remove ?
    explicit Texture(wgpu::TextureDescriptor const&);
    void set_image(uint32_t color_components_count, AlphaSpace, std::span<uint8_t const> data);

    void save(std::filesystem::path const&) const;
    /// NB: the callback will be called asynchronously, several frames later // TODO(WebGPU) for now this isn't the case, we block until it is finished. Should we do async or not ?
    void with_pixels(std::function<void(std::span<uint8_t const>)> const& callback) const;

    auto               imgui_texture_id() const -> ImTextureID { return entire_texture_view().handle(); }
    [[nodiscard]] auto size() const -> img::Size { return img::Size{width(), height()}; }
    [[nodiscard]] auto width() const -> uint32_t { return _desc.size.width; }
    [[nodiscard]] auto height() const -> uint32_t { return _desc.size.height; }
    [[nodiscard]] auto aspect_ratio() const -> float { return static_cast<float>(width()) / static_cast<float>(height()); }
    [[nodiscard]] auto alpha_space() const -> AlphaSpace { return _alpha_space; }

    auto entire_texture_view() const -> TextureView const&; // TODO(WebGPU) When resizing the texture, we probably need to rebuild the view ?
    auto descriptor() const -> wgpu::TextureDescriptor { return _desc; }

private:
    friend class RenderTarget;
    void set_alpha_space(AlphaSpace alpha_space) { _alpha_space = alpha_space; }

private:
    // TODO(ColorSpaces) Store the color space of the texture (Oklab, sRGB, etc.) and use it to check / know if we need conversions etc.
    wgpu::TextureDescriptor            _desc{};
    AlphaSpace                         _alpha_space{};
    mutable std::optional<TextureView> _entire_texture_view{};
};

auto load_texture(std::filesystem::path const& path, wgpu::TextureFormat texture_format, std::optional<AlphaSpace> = {}) -> Texture;
auto texture_from_pixels(img::Size size, wgpu::TextureFormat texture_format, AlphaSpace, std::span<uint8_t const> data) -> Texture;

} // namespace Cool