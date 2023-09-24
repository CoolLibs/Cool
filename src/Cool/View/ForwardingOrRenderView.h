#pragma once
#include "RenderView.h"

namespace Cool {

class ForwardingOrRenderView : public RenderView {
public:
    ForwardingOrRenderView(View const& forwarded_view, ViewCreationParams const& p)
        : RenderView{p}
        , _forwarded_view{forwarded_view}
    {}

private:
    auto get_image_texture_id() const -> ImTextureID override { return _forwarded_view.is_open()
                                                                           ? _forwarded_view.get_image_texture_id()
                                                                           : RenderView::get_image_texture_id(); }
    auto get_image_size() const -> img::Size override { return _forwarded_view.is_open()
                                                                   ? _forwarded_view.get_image_size()
                                                                   : RenderView::get_image_size(); }

private:
    View const& _forwarded_view; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

} // namespace Cool
