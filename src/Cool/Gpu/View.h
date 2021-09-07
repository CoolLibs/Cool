#pragma once
#include "RenderTarget.h"

namespace Cool {

class View {
public:
    View(std::string_view name)
        : _name{name}
    {
    }
    /**
     * @brief Displays the render target's image in an ImGui window
     * 
     * @param name The name of the window
     * @param image_texture_id 
     * @param image_size 
     * @param need_to_fit true iff the window and the image are expected to have different aspect ratios and a fitting operation is required 
     */
    void                     imgui_window(ImTextureID image_texture_id, ImageSize image_size, bool need_to_fit);
    void                     imgui_open_close_checkbox();
    std::optional<ImageSize> size() const { return _size; }
    bool                     is_hovered() const { return _is_hovered; }

private:
    std::string              _name       = "";
    bool                     _is_open    = true;
    bool                     _is_hovered = false;
    std::optional<ImageSize> _size       = std::nullopt; // Can be nullopt when the window is closed
};

} // namespace Cool