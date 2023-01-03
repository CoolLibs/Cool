#include "ImageSizeConstraint.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include "ImageSizeU.h"

namespace Cool {

auto ImageSizeConstraint::applied_to(img::Size frame_size) const -> img::Size
{
    if (wants_to_constrain_size())
    {
        return compute_constraints_on(frame_size);
    }
    else
    {
        return frame_size;
    }
}

auto ImageSizeConstraint::compute_constraints_on(img::Size frame_size) const -> img::Size
{
    float aspect_ratio = _is_controlling_aspect_ratio
                             ? _aspect_ratio.get()
                             : img::SizeU::aspect_ratio(frame_size);

    auto nb_pixels = static_cast<float>(
        _is_controlling_nb_pixels
            ? _nb_pixels
            : frame_size.width() * frame_size.height()
    );

    return {
        std::max(static_cast<img::Size::DataType>(std::round(std::sqrt(nb_pixels * aspect_ratio))), static_cast<img::Size::DataType>(1)),
        std::max(static_cast<img::Size::DataType>(std::round(std::sqrt(nb_pixels / aspect_ratio))), static_cast<img::Size::DataType>(1))};
}

auto ImageSizeConstraint::imgui() -> bool
{
    bool was_triggered = false;
    // Aspect Ratio
    was_triggered |= ImGuiExtras::checkbox_with_submenu("Control aspect ratio", &_is_controlling_aspect_ratio, [&]() {
        return _aspect_ratio.imgui();
    });
    // Nb Pixels
    was_triggered |= ImGuiExtras::checkbox_with_submenu("Control number of pixels", &_is_controlling_nb_pixels, [&]() {
        auto previewNbPixels = _nb_pixels;
        if (ImGuiExtras::slider_uint32("Number of Pixels", &previewNbPixels, 10'000, 500'000))
        {
            _nb_pixels = previewNbPixels;
            return true;
        }
        return false;
    });
    // Interpolation mode
    ImGui::Combo("Interpolation Mode", reinterpret_cast<int*>(&_interpolation_mode), "Nearest\0Linear\0\0");

    return was_triggered;
}

} // namespace Cool