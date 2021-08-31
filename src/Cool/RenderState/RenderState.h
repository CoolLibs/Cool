#pragma once

#include <Cool/Gpu/InterpolationMode.h>
#include <Cool/Image/ImageSize.h>
#include <Cool/Utils/AspectRatio.h>

namespace Cool {

class RenderState {
public:
    static bool is_exporting() { return _is_exporting; }
    static void set_is_exporting(bool is_exporting) { _is_exporting = is_exporting; }
    static void set_export_size(ImageSize size) { _export_size = size; }
    static void set_preview_aspect_ratio(float aspect_ratio)
    {
        _preview_aspect_ratio = aspect_ratio;
        set_is_controlling_preview_aspect_ratio(true);
    }
    static void set_is_controlling_preview_aspect_ratio(bool should_control) { _is_controlling_preview_aspect_ratio = should_control; }
    static void set_preview_nb_pixels(uint32_t nb_pixels) { _preview_nb_pixels = nb_pixels; }
    static void set_is_controlling_preview_nb_pixels(bool should_control) { _is_controlling_preview_nb_pixels = should_control; }

    static bool                     wants_to_constrain_size() { return _is_exporting || _is_controlling_preview_aspect_ratio || _is_controlling_preview_nb_pixels; }
    static std::optional<ImageSize> constrained_size(ImageSize frame_size);
    static ImageSize                preview_size(ImageSize frame_size);
    static ImageSize                export_size() { return _export_size; }

    static void ImGuiPreviewControls();

    /// <summary>
    /// Allows user to control the preview size through the "Preview" menu. It is enabled by default.
    /// </summary>
    static void enablePreviewControlThroughUI();
    /// <summary>
    /// Prevents user from controlling the preview size through the "Preview" menu.
    /// </summary>
    static void disablePreviewControlThroughUI(const char* reasonForDisabling = "");

private:
    static bool              _is_exporting;
    static bool              _is_controlling_preview_nb_pixels;
    static bool              _is_controlling_preview_aspect_ratio;
    static ImageSize         _export_size;
    static InterpolationMode _preview_interpolation_mode;
    static int               _preview_nb_pixels;
    static AspectRatio       _preview_aspect_ratio;

    static bool        m_bPreviewControlThroughUIEnabled;
    static std::string m_bReasonForDisablingPreviewUIControl;
};

} // namespace Cool