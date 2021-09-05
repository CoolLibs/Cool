#pragma once
#include "RenderTarget.h"

namespace Cool {

class RenderTargetWindow {
public:
    void                     imgui_window(std::string_view name);
    std::optional<ImageSize> size() const { return _size; }
    bool                     is_hovered() const { return _is_hovered; }

    void set_is_aspect_ratio_constrained(bool is_constrained) { _is_aspect_ratio_constrained = is_constrained; }

    RenderTarget&       operator*() { return _render_target; }
    RenderTarget*       operator->() { return &_render_target; }
    const RenderTarget& operator*() const { return _render_target; }
    const RenderTarget* operator->() const { return &_render_target; }

private:
    RenderTarget             _render_target;
    std::optional<ImageSize> _size; // Can be nullopt when the window is closed
    bool                     _is_hovered                  = false;
    bool                     _is_aspect_ratio_constrained = false;
};

} // namespace Cool