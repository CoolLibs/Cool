#pragma once
#include <Cool/Gpu/RenderTarget.h>
#include <Cool/Image/ImageSizeConstraint.h>
#include "View.h"

namespace Cool {

struct RenderableView {
    RenderableView(std::string_view name)
        : view{name}, render_target{}
    {
    }

    View         view;
    RenderTarget render_target;

    void update_size(ImageSizeConstraint constraint);
    void imgui_window(ViewWindowParams const& = {});

    auto has_vertical_margins() const -> bool { return view.has_vertical_margins(); }
};

} // namespace Cool
