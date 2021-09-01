#include "PreviewOptions.h"
#include <Cool/ImGuiExtras/ImGuiExtras.h>
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
        std::max(static_cast<ImageSize::DataType>(std::round(std::sqrt(nb_pixels * aspect_ratio))), static_cast<ImageSize::DataType>(1)),
        std::max(static_cast<ImageSize::DataType>(std::round(std::sqrt(nb_pixels / aspect_ratio))), static_cast<ImageSize::DataType>(1))};
}

void PreviewOptions::imgui()
{
    // Aspect Ratio
    ImGuiExtras::checkbox_with_submenu("Control aspect ratio", &_is_controlling_aspect_ratio, [&]() {
        _aspect_ratio.ImGuiPicker(168492224155754);
    });
    // Nb Pixels
    ImGuiExtras::checkbox_with_submenu("Control number of pixels", &_is_controlling_nb_pixels, [&]() {
        int previewNbPixels = _nb_pixels;
        if (ImGui::SliderInt("Number of Pixels", &previewNbPixels, 10000, 100000 /*InAppRenderArea().area()*/)) {
            _nb_pixels = previewNbPixels;
        }
    });
    // Interpolation mode
    ImGui::Combo("Interpolation Mode", reinterpret_cast<int*>(&_interpolation_mode), "Nearest\0Linear\0\0");
}

} // namespace Cool