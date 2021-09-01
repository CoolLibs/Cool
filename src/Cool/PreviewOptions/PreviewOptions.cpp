#include "PreviewOptions.h"
#include <Cool/Image/ImageSizeU.h>

namespace Cool {

std::optional<ImageSize> PreviewOptions::constrained_size(ImageSize frame_size) const
{
    if (wants_to_constrain_size()) {
        return preview_size(frame_size);
    }
    else {
        return std::nullopt;
    }
}

ImageSize PreviewOptions::preview_size(ImageSize frame_size) const
{
    float aspect_ratio = _is_controlling_aspect_ratio
                             ? _aspect_ratio.asFloat()
                             : ImageSizeU::aspect_ratio(frame_size);

    float nb_pixels = static_cast<float>(_is_controlling_nb_pixels
                                             ? _nb_pixels
                                             : frame_size.width() * frame_size.height());

    return {
        static_cast<ImageSize::DataType>(std::round(std::sqrt(nb_pixels * aspect_ratio))),
        static_cast<ImageSize::DataType>(std::round(std::sqrt(nb_pixels / aspect_ratio)))};
}

void PreviewOptions::ImGuiPreviewControls()
{
    // Aspect Ratio
    ImGui::Checkbox("Control aspect ratio", &_is_controlling_aspect_ratio);
    if (_is_controlling_aspect_ratio) {
        _aspect_ratio.ImGuiPicker(168492224155754);
    }
    // Nb Pixels
    ImGui::Checkbox("Control number of pixels", &_is_controlling_nb_pixels);
    if (_is_controlling_nb_pixels) {
        int previewNbPixels = _nb_pixels;
        if (ImGui::SliderInt("Number of Pixels", &previewNbPixels, 10000, 100000 /*InAppRenderArea().area()*/)) {
            _nb_pixels = previewNbPixels;
        }
    }
    // Interpolation mode
    if (_is_controlling_aspect_ratio || _is_controlling_nb_pixels) {
        ImGui::Combo("Interpolation Mode", reinterpret_cast<int*>(&_interpolation_mode), "Nearest\0Linear\0\0");
    }
}

} // namespace Cool