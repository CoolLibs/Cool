#pragma once

#include <Cool/Image/ImageData.h>
#include <Cool/Image/ImageSize.h>
#include "../RenderTargetInfo.h"

namespace Cool {

template<typename RenderTarget_Impl>
class RenderTarget_Base {
public:
    void             render(typename RenderTarget_Impl::RenderFuncType render_fn);
    ImageData        download_pixels() const { return _impl.download_pixels(); }
    ImTextureID      imgui_texture_id() const { return _impl.imgui_texture_id(); }
    RenderTargetInfo info() const { return _impl.info(); }
    ImageSize        current_size() const { return _impl.size(); }
    ImageSize        desired_size() const { return _desired_size; }
    void             set_size(ImageSize size) { _desired_size = size; }

private:
    void resize_if_necessary();

private:
    RenderTarget_Impl _impl;
    ImageSize         _desired_size;
};

} // namespace Cool

#include "RenderTarget_Base.tpp"