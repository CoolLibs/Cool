#pragma once

#include <Cool/Image/ImageData.h>
#include <Cool/Image/ImageSize.h>
#include "../RenderTargetInfo.h"

namespace Cool {

template<typename RenderTarget_Impl>
class RenderTarget_Base {
public:
    void render(RenderTarget_Impl::RenderFuncType render_fn)
    {
        resize_if_necessary();
        _impl.render(render_fn);
    }

    ImageData                download_pixels() const { return _impl.download_pixels(); }
    ImTextureID              imgui_texture_id() const { return _impl.imgui_texture_id(); }
    RenderTargetInfo         info() const { return _impl.info(); }
    ImageSize                current_size() const { return _impl.size(); }
    ImageSize                desired_size() const { return _constrained_size.value_or(ImageSize{}); }
    std::optional<ImageSize> constrained_size() const { return _constrained_size; }
    void                     set_constrained_size(std::optional<ImageSize> size) { _constrained_size = size; }

private:
    void resize_if_necessary();

private:
    RenderTarget_Impl        _impl;
    std::optional<ImageSize> _constrained_size;
};

} // namespace Cool