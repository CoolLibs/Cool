#pragma once
#include <img/img.hpp>
#include "Cool/Path/Path.h"
#include "Cool/WebGPU/Texture.h"
#include "RenderTargetInfo.h"

namespace Cool {

class RenderTarget {
public:
    void        render(std::function<void(wgpu::RenderPassEncoder render_pass)> const& render_fn);
    img::Image  download_pixels() const { return img::load(Cool::Path::root() / "res/images/logo.png"); /*_impl.download_pixels();*/ }
    ImTextureID imgui_texture_id() const { return _texture.imgui_texture_id(); }
    img::Size   current_size() const { return _texture.size(); }
    img::Size   desired_size() const { return _desired_size; }
    void        set_size(img::Size size);
    void        set_size_immediately(img::Size size);
    auto        needs_resizing() const -> bool;

private:
    void resize_if_necessary();

private:
    Texture   _texture;
    img::Size _desired_size{};
};

} // namespace Cool