#pragma once
#include "RenderTarget.h"

namespace Cool {

class View {
public:
    /**
     * @brief Displays the render target's image in an ImGui window
     * 
     * @param name The name of the window
     * @param need_to_fit true iff the window and the render target are expected to have different aspect ratios and a fitting operation is required
     */
    void                     imgui_window(std::string_view name, bool need_to_fit = false);
    std::optional<ImageSize> size() const { return _size; }
    bool                     is_hovered() const { return _is_hovered; }

    void update_render_target_size();
    void set_preview_size(std::optional<ImageSize> size) { _preview_size = size; }

    RenderTarget&       operator*() { return _render_target; }
    RenderTarget*       operator->() { return &_render_target; }
    const RenderTarget& operator*() const { return _render_target; }
    const RenderTarget* operator->() const { return &_render_target; }

private:
    RenderTarget             _render_target;
    bool                     _is_hovered   = false;
    std::optional<ImageSize> _size         = std::nullopt; // Can be nullopt when the window is closed
    std::optional<ImageSize> _preview_size = std::nullopt;
};

} // namespace Cool