#pragma once
#include <Cool/Gpu/RenderTarget.h>
#include <Cool/Image/ImageSizeConstraint.h>
#include "View.h"

namespace Cool {

/// A View that uses a RenderTarget as its image.
/// You can render on that render target however you want.
class RenderView : public View {
public:
    using View::View;

    void update_size(ImageSizeConstraint const&);

    auto render_target() -> RenderTarget& { return _render_target; }
    auto render_target() const -> RenderTarget const& { return _render_target; }

private:
    auto get_image_texture_id() const -> ImTextureID override;
    auto get_image_size_inside_view() const -> ImageSizeInsideView override;

private:
    RenderTarget _render_target;
};

} // namespace Cool
