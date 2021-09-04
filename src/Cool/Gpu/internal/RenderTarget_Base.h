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
    RenderTargetInfo         info() const { return _impl.info(); }
    bool                     is_hovered() const { return _is_hovered; }
    ImageSize                compute_size() const;
    ImageSize                imgui_window_size() const { return _imgui_window_size.value_or(ImageSize{}); }
    std::optional<ImageSize> constrained_size() const { return _constrained_size; }
    void                     set_constrained_size(std::optional<ImageSize> size);
    void                     set_constrained_size(std::optional<ImageSize> size, bool is_aspect_ratio_constrained);

    void imgui_window(std::string_view name) const;

private:
    void resize_if_necessary();

private:
    RenderTarget_Impl                _impl;
    mutable std::optional<ImageSize> _imgui_window_size; // Can be nullopt when the window is closed
    std::optional<ImageSize>         _constrained_size;
    bool                             _is_aspect_ratio_constrained = false;
    mutable bool                     _is_hovered                  = false;
};

} // namespace Cool