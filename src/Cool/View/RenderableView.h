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
    void imgui_window(
        bool aspect_ratio_is_constrained, std::function<bool()> extra_imgui_widgets = []() { return false; });
};

} // namespace Cool
