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

    void set_texture(TextureRef texture) { _texture = texture; }

    auto desired_image_size(ImageSizeConstraint const&) const -> img::Size;

    // auto render_target() -> RenderTarget& { return _render_target; }
    // auto render_target() const -> RenderTarget const& { return _render_target; }

protected:
    auto get_image_texture_id() const -> ImTextureID override;
    auto get_image_size() const -> img::Size override;

private:
    // RenderTarget _render_target;
    TextureRef _texture{};
};

} // namespace Cool
