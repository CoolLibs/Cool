#pragma once

#include <Cool/Gpu/InterpolationMode.h>
#include <Cool/Image/ImageSize.h>
#include <Cool/Utils/AspectRatio.h>

namespace Cool {

class PreviewOptions {
public:
    static PreviewOptions& instance()
    {
        static PreviewOptions render_state;
        return render_state;
    }

    void set_preview_aspect_ratio(float aspect_ratio)
    {
        _preview_aspect_ratio = aspect_ratio;
        set_is_controlling_preview_aspect_ratio(true);
    }
    void set_is_controlling_preview_aspect_ratio(bool should_control) { _is_controlling_preview_aspect_ratio = should_control; }
    void set_preview_nb_pixels(uint32_t nb_pixels) { _preview_nb_pixels = nb_pixels; }
    void set_is_controlling_preview_nb_pixels(bool should_control) { _is_controlling_preview_nb_pixels = should_control; }

    bool                     wants_to_constrain_size() const { return _is_controlling_preview_aspect_ratio || _is_controlling_preview_nb_pixels; }
    std::optional<ImageSize> constrained_size(ImageSize frame_size) const;
    ImageSize                preview_size(ImageSize frame_size) const;

    void ImGuiPreviewControls();

    /// <summary>
    /// Allows user to control the preview size through the "Preview" menu. It is enabled by default.
    /// </summary>
    void enablePreviewControlThroughUI();
    /// <summary>
    /// Prevents user from controlling the preview size through the "Preview" menu.
    /// </summary>
    void disablePreviewControlThroughUI(const char* reasonForDisabling = "");

private:
    bool              _is_controlling_preview_nb_pixels    = false;
    bool              _is_controlling_preview_aspect_ratio = false;
    InterpolationMode _preview_interpolation_mode          = InterpolationMode::NearestNeighbor;
    int               _preview_nb_pixels                   = 250000;
    AspectRatio       _preview_aspect_ratio;

    bool        m_bPreviewControlThroughUIEnabled     = true;
    std::string m_bReasonForDisablingPreviewUIControl = "";
};

} // namespace Cool