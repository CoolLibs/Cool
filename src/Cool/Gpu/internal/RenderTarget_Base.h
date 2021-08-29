#pragma once

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

    RenderTargetInfo info() const { return _impl.info(); }
    ImageSize        compute_size() const;

    void imgui_window(std::string_view name) const;

private:
    void resize_if_necessary();

private:
    RenderTarget_Impl                _impl;
    mutable std::optional<ImageSize> _imgui_window_size; // Can be nullopt when the window is closed
    std::optional<ImageSize>         _imposed_size;
};

} // namespace Cool